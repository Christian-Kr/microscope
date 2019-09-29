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


class LiveCamera;
class QGridLayout;
class ImagePreview;
namespace cv { class Mat; };

///
/// \brief Widget showing all stitching images
///
class StitchingWidget : public  QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Constructor
    /// \param parent Parent widget
    ///
    explicit StitchingWidget(QWidget *parent = nullptr);

    ///
    /// \brief Destructor
    ///
    virtual ~StitchingWidget() override;

    ///
    /// \brief Add an image
    /// \param mat An openvc map of the image
    ///
    void addImage(cv::Mat mat);

    ///
    /// \brief Get the mats vector object
    /// \return The mats vector
    ///
    QVector<cv::Mat> getImages() const;

    ///
    /// \brief Set number of columns
    /// \param rows Number of images in one row
    ///
    void setColumns(int columns);

    ///
    /// \brief Update the size and positions of preview images
    ///
    void updatePreviews();

protected:
    ///
    /// \brief Override resize event method from QWidget
    ///
    virtual void resizeEvent(QResizeEvent *event) override;

    ///
    /// \brief Update the height of the widget
    ///
    virtual void updateHeight();

private:
    QGridLayout *layStitchImages;
    QList<ImagePreview *> *previews;
    QVector<cv::Mat> *mats;
    int columns;
};


#endif // STITCHINGWIDGET_H
