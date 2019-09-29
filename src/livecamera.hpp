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

#ifndef LIVECAMERA_H
#define LIVECAMERA_H

#include <QtCore/QThread>
#include <opencv2/opencv.hpp>


class LiveCamera : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    explicit LiveCamera(QObject *parent = nullptr);

    /**
     * Destructor
     */
    ~LiveCamera();

    /**
     * Set the video capture device
     * @param cap The video capture device
     * @return The old object or nullptr
     */
    cv::VideoCapture* setVideoCaptureDevice(cv::VideoCapture *cap);

public slots:
    /**
     * Show the live camera image
     */
    void runLiveCamera();

    /**
     * Exit loop if running
     */
    void exitLiveCamera();

    /**
     * Get the current live image
     * @return The current live image as cv::Mat
     */
    cv::Mat getCurrentImage();

signals:
    /**
     * Emited when live image has been updated
     */
    void liveImageUpdated();

    /**
     * Exit finish
     */
    void liveCameraExit();

private:
    bool exit;

    cv::Mat *liveImage;
    cv::VideoCapture *videoCapture;
};


#endif // LIVECAMERA_H
