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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtCore/QObject>


class QSerialPort;

enum class Direction { LEFT, RIGHT };
enum class MotorNumber { ONE, TWO };

/**
 * Class for working with the microscope controller
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     */
    explicit Controller(QObject *parent = nullptr);

    /**
     * Destructor
     */
    virtual ~Controller();

    /**
     * Connect to controller
     */
    bool connectPort();

    /**
     * Disconnect the port if it is connected
     */
    void disconnectPort();

    /**
     * Set the device path
     * @param device Path to device
     */
    void setDevice(const QString &device);

    /**
     * Get the device path
     * @return The device path as string, currently set
     */
    QString getDevice();

    /**
     * Get a list of available port names
     * @return A list ov available port names as strings
     */
    QList<QString> availablePortNames();

    /**
     * Move motor for one intervall
     * @param direction LEFT or RIGHT
     * @param motor THe motor number to move
     */
    void moveMotor(Direction direction, MotorNumber motor);

    /**
     * Set the motor intervall for every move
     * @param stepsPerMoveX The steps per move for the first motor
     * @param stepsPerMoveY The steps per move for the second motor
     */
    void setMotorIntervall(int stepsPerMoveX, int stepsPerMoveY);

    /**
     * Reset the controller
     */
    void reset();

    /**
     * Move to the next position
     */
    void moveToNextPos();

    /**
     * Has reached position end
     * @return True if reached position end, else false
     */
    bool hasReachedPosEnd();

    /**
     * Set the maximum allowed moves per axis
     * @param maxMovesX Maximum moves in x position
     * @param maxMovesY Maximum moves in y position
     */
    void setMaxMoves(int maxMovesX, int maxMovesY);

public slots:
    /**
     * Read data
     */
    void readData();

signals:
    /**
     * The controller is ready to handle a command
     */
    void ready();

private:
    QString device;
    bool portConnected;
    int stepsPerMoveX;
    int stepsPerMoveY;;
    int maxMovesX;
    int maxMovesY;
    int currPosX;
    int currPosY;
    bool waiting;

    QSerialPort *serialPort;
    QByteArray cmdBuffer;
    QString cmd;
};


#endif // CONTROLLER_H
