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

#include "imagecalibration.hpp"
#include <math.h>

#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>


ImageCalibration::ImageCalibration(QWidget *parent)
    : ImagePreview(parent),
    xStart(0),
    yStart(0),
    xEnd(0),
    yEnd(0),
    lineLength(0)
{
}

ImageCalibration::ImageCalibration(QString label, QWidget *parent)
    : ImagePreview(label, parent),
    xStart(0),
    yStart(0),
    xEnd(0),
    yEnd(0),
    lineLength(0)
{
}

ImageCalibration::~ImageCalibration()
{
}


void ImageCalibration::paintEvent(QPaintEvent *event)
{
    ImagePreview::paintEvent(event);

    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawLine(xStart, yStart, xEnd, yEnd);
}

void ImageCalibration::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "ESC gedrückt";
    }
}

void ImageCalibration::mousePressEvent(QMouseEvent *event)
{
     if (event->button() == Qt::MouseButton::LeftButton) {
         xStart = event->x();
         yStart = event->y();
     } else {
         ImagePreview::mousePressEvent(event);
     }
}

void ImageCalibration::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton) {
        qDebug() << "Mouse move";
        xEnd = event->x();
        yEnd = event->y();
        repaint();
    } else {
        ImagePreview::mouseMoveEvent(event);
    }

    double x = xEnd - xStart;
    double y = yEnd - yStart;
    lineLength = sqrt(pow(x, 2) + pow(y, 2));
}

void ImageCalibration::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        double x = xEnd - xStart;
        double y = yEnd - yStart;
        lineLength = sqrt(pow(x, 2) + pow(y, 2));
        repaint();
        emit measurementFinished(lineLength);
    } else {
        ImagePreview::mouseReleaseEvent(event);
    }
}

double ImageCalibration::getLineLength()
{
    return lineLength;
}
