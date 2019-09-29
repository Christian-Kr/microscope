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


/**
 * Image preview of the so far calculated one
 */ 
class ImagePreview : public  QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    explicit ImagePreview(QWidget *parent = nullptr);

    /**
     * Overloaded constructor
     * @param parent Parent widget
     * @param label Label of the widget
     */
    explicit ImagePreview(QString label, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    virtual ~ImagePreview();

    /**
     * Set the pixmap to preview
     * @param pix THe pixmap object
     */
    void setPixmap(QPixmap &pix);

    /**
     * Return the currently set pixmap
     * @return Pixmap copy
     */
    QPixmap getPixmap();

    /**
     * Set label to draw
     */
    void setLabel(QString label);

protected:
    /**
     * Override the paint event method to draw the image
     * @param event The paint event
     */
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QPixmap *pix;
    QString label;
};


#endif // IMAGEPREVIEW_H
