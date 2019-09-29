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

#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include <QtWidgets/QWidget>


///
/// \brief Image preview of the so far calculated one
///
class ImagePreview : public  QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Constructor
    /// \param parent Parent widget
    ///
    explicit ImagePreview(QWidget *parent = nullptr);

    ///
    /// \brief Overloaded constructor
    /// \param parent Parent widget
    /// \param label Label of the widget
    ///
    explicit ImagePreview(QString label, QWidget *parent = nullptr);

    ///
    /// \brief Destructor
    ///
    virtual ~ImagePreview() override;

    ///
    /// \brief Set the pixmap to preview
    /// \param pix THe pixmap object
    ///
    void setPixmap(QPixmap &pix);

    ///
    /// \brief Return the currently set pixmap
    /// \return Pixmap copy
    ///
    QPixmap getPixmap();

    ///
    /// \brief Set label to draw
    ///
    void setLabel(QString label);

signals:
    ///
    /// \brief Pixmap height changed
    /// \param height The new height of the scaled image
    ///
    void pixmapHeightChanged(int height);

protected:
    ///
    /// \brief Override the paint event method to draw the image
    /// \param event The paint event
    ///
    virtual void paintEvent(QPaintEvent *) override;

    ///
    /// \brief Override the close event method
    /// \param event The close event
    ///
    virtual void closeEvent(QCloseEvent *) override;

private:
    QPixmap *pix;
    QPixmap *pixScaled;
    QString label;
};


#endif // IMAGEPREVIEW_H
