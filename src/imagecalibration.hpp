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

#ifndef IMAGECALIBRATION_H
#define IMAGECALIBRATION_H

#include "imagepreview.hpp"


/**
 * Image calibration
 */ 
class ImageCalibration : public  ImagePreview
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    explicit ImageCalibration(QWidget *parent = nullptr);

    /**
     * Overloaded constructor
     * @param parent Parent widget
     * @param label Label of the widget
     */
    explicit ImageCalibration(QString label, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    virtual ~ImageCalibration();

    /**
     * Get the calculated line length in unit pixles
     * @return Line length in pixels
     */
    double getLineLength();

signals:
    /**
     * Emits when measurement has been finished
     * @param lineLength The line length for calibration per pixels
     */
    void measurementFinished(double lineLength);

protected:
    /**
     * Override function from QWidget
     */
    virtual void paintEvent(QPaintEvent *event) override;

    /**
     * Override function from QWidget
     */
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    /**
     * Override function from QWidget
     */
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * Override function from QWidget
     */
    virtual void mousePressEvent(QMouseEvent *event) override;

    /**
     * Override function from QWidget
     */
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    /**
     * Variables for save start and end position
     */
    double xStart;
    double yStart;
    double xEnd;
    double yEnd;

    double lineLength;
};


#endif // IMAGECALIBRATION_H
