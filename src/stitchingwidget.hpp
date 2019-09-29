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

#ifndef STITCHINGWIDGET_H
#define STITCHINGWIDGET_H

#include <QtWidgets/QWidget>

#include "ui_stitchingwidget.h"


class LiveCamera;
class QGridLayout;
class ImagePreview;
namespace cv { class Mat; };

/**
 * Widget showing all stitching images and buttons to handle them properly
 */
class StitchingWidget : public  QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    explicit StitchingWidget(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    virtual ~StitchingWidget();

private:
    QGridLayout *layStitchImages;
    QVector<cv::Mat> *mats;
};


#endif // STITCHINGWIDGET_H
