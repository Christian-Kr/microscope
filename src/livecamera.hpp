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


///
/// \brief The LiveCamera class, showing a live image from the usb-camera
/// The camera stream will be catched from opencv VideoCapture object. There is
/// no own driver or streaming implementation.
///
class LiveCamera : public QObject
{
    Q_OBJECT

public:
    ///
    /// \brief Constructor
    /// \param parent Parent object
    ///
    explicit LiveCamera(QObject *parent = nullptr);

    ///
    /// \brief Destructor
    ///
    virtual ~LiveCamera() override;

    ///
    /// \brief Set the video capture device
    /// \param cap The video capture device
    /// \return The old object or nullptr
    ///
    cv::VideoCapture* setVideoCaptureDevice(cv::VideoCapture *cap);

public slots:
    ///
    /// \brief Show the live camera image
    ///
    void runLiveCamera();

    ///
    /// \brief Exit loop if running
    ///
    void exitLiveCamera();

    ///
    /// \brief Get the current live image
    /// \return The current live image as cv::Mat
    ///
    cv::Mat getCurrentImage();

signals:
    ///
    /// \brief Emited when live image has been updated
    ///
    void liveImageUpdated();

    ///
    /// \brief Exit finish
    ///
    void liveCameraExit();

private:
    bool exit;
    cv::Mat *liveImage;
    cv::VideoCapture *videoCapture;
};


#endif // LIVECAMERA_H
