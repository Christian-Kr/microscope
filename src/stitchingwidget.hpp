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

    ///
    /// \brief Get the selected image preview object
    /// \return Pointer to image preview object or nullptr if nothing is
    /// selected
    ///
    ImagePreview* getSelectedImagePreview();

    ///
    /// \brief Get the selected image as opencv matrice
    /// \return Selected image object. This method will always return an object
    /// no matter if one is selected or not. You may check with mat.empty if
    /// one is selected, or with isSelected metohd.
    ///
    cv::Mat getSelectedImage();

    ///
    /// \brief Get info if image is selected
    /// \return True if one image is selected, else false
    ///
    bool isImageSelected() const;

    ///
    /// \brief Remove given image
    /// \param imagePreview The image preview object
    /// \return True if remove successfull, else false
    ///
    bool removeImage(ImagePreview *imagePreview);

public slots:
    ///
    /// \brief Selection changed by mouse click
    /// \param state The new selection state
    ///
    void selectionChangedByMouse(bool state);

    /// 
    /// \brief Show a single preview of emited preview
    ///
    void showSinglePreview();

signals:
    ///
    /// \brief Emited when the selected element changed
    ///
    void selectedImagePreviewChanged();

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
    ///
    /// \brief Deselect all previews
    ///
    void deselectAll();

    QGridLayout * layStitchImages;
    QList<ImagePreview *> * previews;
    QVector<cv::Mat> * mats;
    ImagePreview * selected;
    ImagePreview * previewSingle;
    int columns;
};


#endif // STITCHINGWIDGET_H
