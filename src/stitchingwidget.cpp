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


#include <QtWidgets/QGridLayout>
#include <opencv2/opencv.hpp>
#include <QtCore/QVector>
#include <QtCore/QDebug>

#include "stitchingwidget.hpp"
#include "livecamera.hpp"
#include "imagepreview.hpp"
#include "mainwin.hpp"


StitchingWidget::StitchingWidget(QWidget *parent)
    : QWidget(parent),
    layStitchImages(new QGridLayout(this)),
    previews(new QList<ImagePreview *>()),
    mats(new QVector<cv::Mat>()),
    columns(5),
    selected(nullptr)
{
}

StitchingWidget::~StitchingWidget()
{
    delete layStitchImages;
    delete mats;

    while (!previews->isEmpty())
        delete previews->takeFirst();
    delete previews;
}

void StitchingWidget::addImage(cv::Mat mat)
{
    mats->append(mat);

    // Get pixmap of the mat object and add it as new preview
    QPixmap pix = MainWin::matToPixmap(mat);
    ImagePreview *preview = new ImagePreview(this);
    preview->setVisible(true);
    preview->setPixmap(pix);
    previews->append(preview);
    connect(
        preview, &ImagePreview::selectionChanged, this, 
        &StitchingWidget::selectionChangedByMouse
    );
    updatePreviews();
}

void StitchingWidget::selectionChangedByMouse(bool state)
{
    if (!state) {
        selected = nullptr;
        return;
    }
    
    QObject *obj = sender();
    ImagePreview *preview = qobject_cast<ImagePreview *>(obj);
    if (preview == nullptr)
        return;
    for (int i = 0; i < previews->size(); i++) {
        if (previews->at(i) != preview)
            previews->at(i)->setSelected(false);
    }

    if (selected != preview) {
        selected = preview;
        emit selectedImagePreviewChanged();
    }
}

void StitchingWidget::deselectAll()
{
    for (int i = 0; i < previews->size(); i++) {
        previews->at(i)->setSelected(false);
    }
}

void StitchingWidget::updatePreviews()
{
    // The width is given by the number of columns and the total width of
    // the widget.
    int imageWidth = width() / columns;
    for (int i = 0; i < previews->size(); i++) {
        ImagePreview *preview = previews->at(i);
        int row = static_cast<int>(i / columns);
        int column = i - (row * columns);
        preview->setGeometry(
            column * imageWidth,
            row * 200,
            imageWidth,
            200
        );
    }
    updateHeight();
}

void StitchingWidget::updateHeight()
{
    if (!previews->isEmpty()) {
        int rows = static_cast<int>(previews->size() / columns);
        setGeometry(0, 0, width(), (rows + 1) * previews->first()->height());
    }
}

void StitchingWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updatePreviews();
}

QVector<cv::Mat> StitchingWidget::getImages() const
{
    return *mats;
}

void StitchingWidget::setColumns(int columns)
{
    this->columns = columns;
}

ImagePreview* StitchingWidget::getSelectedImagePreview()
{
    return selected;
}
