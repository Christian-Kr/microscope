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

#include "imagepreview.hpp"

#include <QtGui/QPainter>


ImagePreview::ImagePreview(QWidget *parent)
    : QWidget(parent),
    pix(new QPixmap())
{
}

ImagePreview::ImagePreview(QString label, QWidget *parent)
    : QWidget(parent),
    pix(new QPixmap())
{
    this->label = label;
}

ImagePreview::~ImagePreview()
{
    delete pix;
}

QPixmap ImagePreview::getPixmap()
{
    return *pix;
}

void ImagePreview::setLabel(QString label)
{
    this->label = label;
}

void ImagePreview::paintEvent(QPaintEvent *event)
{
    if (pix->height() <= 0)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int labelHeight = 60;

    // Pix data
    double pixRatio = pix->width() / pix->height();
    double pixWidth = pix->width();
    double pixHeight = pix->height();

    // Full size data
    double fullHeight = this->height();
    double fullWidth = this->width();

    // Widget data
    double winHeight = this->height() - labelHeight;
    double winWidth = this->width();

    // Data for draw
    double x, y;
    
    // Calculate data and draw the images
    if (pixWidth < winWidth && pixHeight < winHeight) {
        // The pixmap is smaller than the widget. Center the pixmap and leave
        // the size as it is.
        x = (winWidth - pixWidth) / 2;
        y = (winHeight - pixHeight) / 2 + labelHeight;
        painter.drawPixmap(x, y, *pix);
    } else {
        // The pixmaps' width or height is bigger than the widgets one. Scale
        // the image down to fit the bigger side and center the other one.
        QPixmap tmpPix;
        if ((pixWidth - winWidth) > (pixHeight - winHeight)) {
            // The width is higher as the border than the height
            tmpPix = pix->scaledToWidth(
                winWidth, Qt::TransformationMode::SmoothTransformation
            );
            x = 0;
            y = (winHeight - tmpPix.height()) / 2 + labelHeight;
        } else {
            // The height is higher as the border than the width
            tmpPix = pix->scaledToHeight(
                winHeight, Qt::TransformationMode::SmoothTransformation
            );
            x = (winWidth - tmpPix.width()) / 2;
            y = labelHeight;
        }
        painter.drawPixmap(x, y, tmpPix);
    }

    // Draw a border around the image
    QPen pen;
    pen.setColor(QColor("gray"));
    pen.setWidth(5);
    painter.setPen(pen);
    painter.drawRect(0, 0, fullWidth, fullHeight);
    pen.setColor(QColor("black"));
    painter.setPen(pen);
    painter.drawText(20, labelHeight / 2, label);
}

void ImagePreview::setPixmap(QPixmap &pix)
{
    *(this->pix) = pix;
    repaint();
}
