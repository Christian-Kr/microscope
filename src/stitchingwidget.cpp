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


StitchingWidget::StitchingWidget(QWidget *parent)
    : QWidget(parent),
    layStitchImages(new QGridLayout(this)),
    mats(new QVector<cv::Mat>())
{
}

StitchingWidget::~StitchingWidget()
{
    delete layStitchImages;
    delete mats;
}
