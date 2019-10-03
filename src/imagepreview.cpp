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

#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>

#include "imagepreview.hpp"


ImagePreview::ImagePreview(QWidget *parent, bool closeable)
    : QWidget(parent),
    pix(new QPixmap()),
    pixScaled(new QPixmap()),
    closeable(closeable),
    selected(false)
{
}

ImagePreview::ImagePreview(QString label, QWidget *parent, bool closeable)
    : QWidget(parent),
    pix(new QPixmap()),
    pixScaled(new QPixmap()),
    closeable(closeable),
    selected(false)
{
    this->label = label;
}

ImagePreview::~ImagePreview()
{
    delete pix;
    delete pixScaled;
}

QPixmap ImagePreview::getPixmap()
{
    return *pix;
}

void ImagePreview::setLabel(QString label)
{
    this->label = label;
}

void ImagePreview::paintEvent(QPaintEvent *)
{
    if (pix->height() <= 0 || pix->width() <= 0)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int border = 20;
    double pixWidth = pix->width();
    double pixHeight = pix->height();
    double winWidth = width() - border;
    double winHeight = height() - border;

    // Calculate data and draw the images
    if (pixWidth <= winWidth) {
        painter.drawPixmap(
            static_cast<int>((winWidth - pixWidth) / 2),
            static_cast<int>((winHeight - pixHeight) / 2),
            *pix
        );
    } else {
        *pixScaled = pix->scaledToWidth(
            static_cast<int>(winWidth),
            Qt::TransformationMode::SmoothTransformation
        );
        painter.drawPixmap(
            static_cast<int>(
                (winWidth - pixScaled->width()) / 2 + border / 2
            ),
            static_cast<int>(
                (winHeight - pixScaled->height()) / 2 + border / 2
            ),
            *pixScaled
        );
    }

    // Paint border if selected
    if (selected) {
        QPen pen;
        pen.setColor(Qt::darkGray);
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawRect(0, 0, width(), height());
    }

    if (height() != pixScaled->height())
        emit pixmapHeightChanged(pixScaled->height());
}

void ImagePreview::setPixmap(QPixmap & pix)
{
    *(this->pix) = pix;
    repaint();
}

void ImagePreview::closeEvent(QCloseEvent * event)
{
    // Do nothing! Tis window should not be closeable by the window system!
    if (!closeable)
        event->ignore();
}

void ImagePreview::setSelected(bool state)
{
    selected = state;
    repaint();
}

bool ImagePreview::isSelected() const
{
    return selected;
}

void ImagePreview::mousePressEvent(QMouseEvent * event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton) {
        // Only select a deselected element, but don't deselect a selected one
        if (!isSelected()) {
            setSelected(true);
            emit selectionChanged(true);
        }
    } else {
        QWidget::mousePressEvent(event);
    }
}

void ImagePreview::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton) {
        emit showSinglePreview();
    } else {
        QWidget::mouseDoubleClickEvent(event);
    }
}
