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
    explicit ImagePreview(
        QWidget *parent = nullptr, bool closeable = true);

    ///
    /// \brief Overloaded constructor
    /// \param parent Parent widget
    /// \param label Label of the widget
    ///
    explicit ImagePreview(
        QString label, QWidget *parent = nullptr, bool closeable = true);

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

    ///
    /// \brief Set the selection date
    /// \param state True if selected, else false
    ///
    void setSelected(bool state);

    ///
    /// \brief Selection state
    /// \return True if selected, else false
    ///
    bool isSelected() const;

signals:
    ///
    /// \brief Pixmap height changed
    /// \param height The new height of the scaled image
    ///
    void pixmapHeightChanged(int height);

    ///
    /// \brief Emited when selection state changed
    /// \param state The new state
    ///
    void selectionChanged(bool state);

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

    ///
    /// \brief Override mouse press event method
    /// \param event The mouse press event
    ///
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    QPixmap *pix;
    QPixmap *pixScaled;
    QString label;
    bool closeable;
    bool selected;
};


#endif // IMAGEPREVIEW_H
