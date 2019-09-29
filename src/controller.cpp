//
// Copyright (¢) 2019 by Christian Krippendorf
//
// This file is part of microscope.
//
// microscope is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// microscope is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with microscope. If not, see <http://www.gnu.org/licenses/>.
//

#include "controller.hpp"
#include "mainwin.hpp"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>


Controller::Controller(QObject *parent)
    : QObject(parent),
    portConnected(false),
    serialPort(new QSerialPort()),
    stepsPerMoveX(0),
    stepsPerMoveY(0),
    maxMovesX(0),
    maxMovesY(0),
    currPosX(0),
    currPosY(0),
    waiting(false)
{
    // Connections
    connect(serialPort, &QSerialPort::readyRead, this, &Controller::readData);
}

Controller::~Controller()
{
    delete serialPort;
}

bool Controller::connectPort()
{
    // Is the port already open?
    if (portConnected) {
        QMessageBox::information(
            MainWin::instance(), tr("Connect port"),
            tr("Port is already connected.")
        );
        return false;
    }

    QList<QSerialPortInfo> portInfos = QSerialPortInfo::availablePorts();
    QSerialPortInfo portInfo;
    bool portFound = false;
    for (int i = 0; i < portInfos.size(); i++) {
        if (portInfos.at(i).systemLocation() == device) {
            portInfo = portInfos.at(i);
            portFound = true;
            break;
        }
    }

    if (!portFound) {
        QMessageBox::critical(
            MainWin::instance(), tr("Connect port"),
            tr("Port is not available!")
        );
        return false;
    }
    
    serialPort->setPort(portInfo);
    if (!serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(
            MainWin::instance(), tr("Connect port"),
            tr("Cannot open port!")
        );
        return false;
    }

    portConnected = true;

    return true;
}

void Controller::setMotorIntervall(int stepsPerMoveX, int stepsPerMoveY)
{
    this->stepsPerMoveX = stepsPerMoveX;
    this->stepsPerMoveY = stepsPerMoveY;
}

void Controller::moveMotor(Direction direction, MotorNumber motor)
{
    if (!portConnected) {
        qDebug() << "Port is not connected for a move!";
        return;
    }

    waiting = true;

    switch (motor) {
    case MotorNumber::ONE:
        serialPort->write("M1\n");
        serialPort->waitForBytesWritten();
        switch (direction) {
        case Direction::LEFT:
            serialPort->write(QString("-%1\n").arg(stepsPerMoveX).toLatin1());
            serialPort->waitForBytesWritten();
            break;
        case Direction::RIGHT:
            serialPort->write(QString("%1\n").arg(stepsPerMoveX).toLatin1());
            serialPort->waitForBytesWritten();
            break;
        default:
            return;
        }
        break;
    case MotorNumber::TWO:
        serialPort->write("M2\n");
        serialPort->waitForBytesWritten();
        switch (direction) {
        case Direction::LEFT:
            serialPort->write(QString("-%1\n").arg(stepsPerMoveY).toLatin1());
            serialPort->waitForBytesWritten();
            break;
        case Direction::RIGHT:
            serialPort->write(QString("%1\n").arg(stepsPerMoveY).toLatin1());
            serialPort->waitForBytesWritten();
            break;
        default:
            return;
        }
        break;
    default:
        return;
    }
}

void Controller::disconnectPort()
{
    serialPort->close();
    portConnected = false;
}

void Controller::setDevice(const QString &device)
{
    this->device = device;
}

QString Controller::getDevice()
{
    return device;
}

QList<QString> Controller::availablePortNames()
{
    QList<QString> names;
    QList<QSerialPortInfo> portInfos = QSerialPortInfo::availablePorts();
    for (int i = 0; i < portInfos.size(); i++) {
        names.append(portInfos.at(i).systemLocation());
    }
    return names;
}

void Controller::readData()
{
    QByteArray data = serialPort->readAll();
    if (!data.contains('\n')) {
        cmdBuffer.append(data);
        return;
    } else {
        int idx = data.indexOf('\n');
        cmdBuffer.append(data.left(idx));
        cmd = QString(cmdBuffer);
        cmdBuffer.clear();
        cmdBuffer.append(data.right(data.length()-idx-1));
        if (cmd == "READY") {
            waiting = false;
            qDebug() << cmd;
            qDebug() << cmdBuffer;
            cmd.clear();
            emit ready();
        }
    }
}

void Controller::reset()
{
    currPosX = 0;
    currPosY = 0;
}

void Controller::moveToNextPos()
{
    qDebug() << "Move to next position";

    // Don't send any command, if controller is still waiting
    if (waiting)
        return;

    // Switch direction on odd X numbers
    Direction directionTWO = Direction::RIGHT;
    if (currPosX % 2 != 0)
        directionTWO = Direction::LEFT;

    if (currPosY == maxMovesY && currPosX < maxMovesX) {
        moveMotor(Direction::LEFT, MotorNumber::ONE);
        currPosX++;
        currPosY = 0;
        return;
    }
    if (currPosY < maxMovesY) {
        moveMotor(directionTWO, MotorNumber::TWO);
        currPosY++;
        return;
    }
    if (currPosY == maxMovesY && currPosX == maxMovesX) {
        // Reached end of positioning
        return;
    }
}

bool Controller::hasReachedPosEnd()
{
    if (currPosX == maxMovesX && currPosY == maxMovesY)
        return true;
    return false;
}

void Controller::setMaxMoves(int maxMovesX, int maxMovesY)
{
    this->maxMovesX = maxMovesX;
    this->maxMovesY = maxMovesY;
}
