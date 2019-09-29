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

#include "livecamera.hpp"

#include <QtCore/QDebug>


LiveCamera::LiveCamera(QObject *parent)
    : QObject(parent),
    exit(false),
    liveImage(new cv::Mat()),
    videoCapture(nullptr)
{
}

LiveCamera::~LiveCamera()
{
    delete liveImage;
}

void LiveCamera::runLiveCamera()
{
    if (videoCapture == nullptr) {
        qDebug() << "No video capture device set";
        return;
    }

    exit = false;
    while (!exit) {
        *videoCapture >> *liveImage;
        if (liveImage->empty())
            break;

        emit liveImageUpdated();
    }
    emit liveCameraExit();
}

void LiveCamera::exitLiveCamera()
{
    exit = true;
}

cv::Mat LiveCamera::getCurrentImage()
{
    return *liveImage;
}

cv::VideoCapture* LiveCamera::setVideoCaptureDevice(cv::VideoCapture *cap)
{
    cv::VideoCapture *tmp = this->videoCapture;
    this->videoCapture = cap;
    return tmp;
}