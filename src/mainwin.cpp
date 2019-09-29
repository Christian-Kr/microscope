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

#include <opencv2/opencv.hpp>
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QList>
#include <QtCore/QDebug>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QSettings>
#include <QtWidgets/QAction>
#include <QtWidgets/QSpacerItem>

#include "mainwin.hpp"
#include "livecamera.hpp"
#include "imagepreview.hpp"
#include "controller.hpp"
#include "stitchingwidget.hpp"


// Initialize the singleton instance for working with it in static functions
MainWin* MainWin::_instance = nullptr;

MainWin::MainWin(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
    mats(new QVector<cv::Mat>()),
    cap(nullptr),
    cameraConnected(false),
    controllerConnected(false),
    thread(new QThread()),
    liveCamera(new LiveCamera()),
    guiMode(GuiMode::NORMAL),
    preview(new ImagePreview(tr("Normal"), this)),
    previewLiveCamera(new ImagePreview(tr("Live Camera"), this)),
    layoutMain(new QHBoxLayout()),
    layoutImages(new QGridLayout()),
    controller(new Controller()),
    gridNumMaxX(5),
    gridNumMaxY(5)
{
    ui.setupUi(this);

    liveCamera->moveToThread(thread);
    thread->start();

    QVBoxLayout *layoutLivePreview = new QVBoxLayout(this);
    layoutLivePreview->addWidget(previewLiveCamera);
    layoutLivePreview->addSpacerItem(
        new QSpacerItem(
            100, 100, QSizePolicy::Policy::Expanding,
            QSizePolicy::Policy::Expanding
        )
    );
    layoutMain->addLayout(layoutLivePreview);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setLayout(layoutImages);
    layoutMain->addWidget(scrollArea);
    centralWidget()->setLayout(layoutMain);

    preview->setVisible(false);
    previewLiveCamera->setVisible(false);
    previewLiveCamera->setMinimumSize(QSize(400, 400));

    // Update gui elements
    ui.tbCamera->setVisible(false);

    // Load different main window settings
    QSettings settings;
    setGeometry(
        x(), y(),
        settings.value("window_width", 1000).toInt(),
        settings.value("window_height", 1000).toInt()
    );

    updateRecentMenu();
    buildConnections();
}

MainWin::~MainWin()
{
    thread->quit();
    thread->wait();

    delete thread;
    delete liveCamera;
    delete mats;
    delete controller;

    // Delete opencv objects
    delete cap;
    delete mats;
}

void MainWin::buildConnections()
{
    connect(
        this, &MainWin::runLiveCamera, liveCamera,
        &LiveCamera::runLiveCamera
    );
    connect(
        liveCamera, &LiveCamera::liveImageUpdated,
        this, &MainWin::liveImageUpdated
    );
    connect(
        liveCamera, &LiveCamera::liveCameraExit,
        this, &MainWin::liveCameraExit
    );
    connect(
        thread, &QThread::finished, liveCamera, &QObject::deleteLater
    );

    connect(
        controller, &Controller::ready, this, &MainWin::controllerReady
    );
}

bool MainWin::initCamera()
{
    // The camera will be initialized with opencv. There might be an option to
    // detect it with the help of qt. But this won't be used for now.

    if (cap == nullptr) {
        // Try to get the first camera
        cap = new cv::VideoCapture(0);
    }

    if (!cap->isOpened()) {
        QMessageBox::critical(
            this, tr("Initialize camera"),
            tr("Cannot connect the camera!")
        );
        return false;
    }
    statusBar()->showMessage(tr("Camera connected!"));
    return true;
}

bool MainWin::initController()
{
    controller->setDevice("/dev/ttyACM0");
    if (!controller->connectPort()) {
        QMessageBox::critical(
            this, tr("Initialize camera"),
            tr("Cannot connect the controller!")
        );
        return false;
    }
    statusBar()->showMessage(tr("Controller connected!"));
    return true;
}

void MainWin::connectController()
{
    if (controllerConnected) {
        // Disconnect
        controller->disconnectPort();
        ui.actConnController->setChecked(false);
        controllerConnected = false;
    } else {
        // Connect
        controllerConnected = initController();
        ui.actConnController->setChecked(controllerConnected);
    }
}

void MainWin::connectCamera()
{
    if (cameraConnected) {
        // Disconnect
        liveCamera->exitLiveCamera();

        // After the camera has been connected, start to show the live view.
        ui.tbCamera->setVisible(false);
        previewLiveCamera->setVisible(false);
    } else {
        // Connect
        cameraConnected = initCamera();
        if (!cameraConnected) {
            ui.actConnCamera->setChecked(false);
            return;
        }

        // After the camera has been connected, start to show the live view.
        ui.tbCamera->setVisible(true);
        previewLiveCamera->setVisible(true);
        liveCamera->setVideoCaptureDevice(cap);
        emit runLiveCamera();
    }
}

void MainWin::connectDevices()
{
    if (!controllerConnected)
        connectController();
    if (!cameraConnected)
        connectCamera();
}

void MainWin::liveImageUpdated()
{
    cv::Mat tmp = liveCamera->getCurrentImage();
    QPixmap pix = MainWin::matToPixmap(tmp);
    previewLiveCamera->setPixmap(pix);
}

void MainWin::liveCameraExit()
{
    cap->release();
    delete cap;
    cap = nullptr;
    cameraConnected = false;
    ui.actConnCamera->setChecked(false);
}

void MainWin::takeImage()
{
    qDebug() << "Here";
    cv::Mat liveMat;
    liveCamera->getCurrentImage().copyTo(liveMat);
    mats->push_back(liveMat);

    // Create a new preview for the image and add it to the layout
    ImagePreview *preview = new ImagePreview(
        QString("Image no. %1").arg(mats->size()),
        this
    );
    QPixmap pixTmp = MainWin::matToPixmap(liveMat);
    preview->setPixmap(pixTmp);

    // Get the number of current previews
    int numCurrPrev = mats->size() - 1;
    int y = static_cast<int>(numCurrPrev / gridNumMaxY);
    int x = numCurrPrev - (y * gridNumMaxY);

    layoutImages->addWidget(preview, y, x);
    preview->setFixedSize(QSize(200, 200));
}

// ---- NEW NEW NEW

void MainWin::runAutoCameraStitching()
{
    if (!cameraConnected) {
        cameraConnected = initCamera();
        if (!cameraConnected)
            return;
    }
    if (!controllerConnected) {
        controllerConnected = initController();
        if (!controllerConnected)
            return;
    }

    // Change gui to auto camera stitching mode.
    guiMode = GuiMode::AUTOMATIC_CAMERA_STITCHING;

    // The auto camera stitching mode works mainly as the manual mode, extended
    // by an automatic camera moving mode. Therefore some parameter are needed
    // to determin how much every part of the motor should run. Additionally
    // the system needs to know what position both motors have on start. This
    // should be controlled directly on the arduino firmware wil electronic
    // sensors.

    // How much should everything move in both direction. One move is defined
    // by a camera step with a fixed motor move. For 360° the motor here has
    // 2048 steps.
    int maxMovesY = 6;
    int maxMovesX = 4;

    // Steps per move
    int stepsPerMoveX = 80;
    int stepsPerMoveY = 400;

    // Set video capture device
    liveCamera->setVideoCaptureDevice(cap);

    // Run the live camera
    emit runLiveCamera();
    QThread::sleep(2);

    // Empty the picture buffer
    mats->clear();

    // Send a reset signal to the controller
    controller->reset();
    controller->setMotorIntervall(stepsPerMoveX, stepsPerMoveY);
    controller->setMaxMoves(maxMovesX, maxMovesY);

    // Start by moving motor once
    controller->moveToNextPos();
}

void MainWin::runCameraStitching()
{
    // TODO: Do camera stitching

    if (cameraConnected == false) {
        cameraConnected = initCamera();
        if (!cameraConnected)
            return;
    }
}

void MainWin::abortCameraStitching()
{
    liveCamera->exitLiveCamera();
    guiMode = GuiMode::NORMAL;
}

void MainWin::abortAutoCameraStitching()
{
    if (guiMode != GuiMode::AUTOMATIC_CAMERA_STITCHING)
        return;

    liveCamera->exitLiveCamera();
    guiMode = GuiMode::NORMAL;
}

void MainWin::stitchImages()
{
    QPixmap tmpPix;
    cv::Mat stitchedMat;

    // No need to stitch, when there is only one in pipe
    if (mats->size() > 1) {
        // Stitch images
        cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(
            cv::Stitcher::SCANS, false
        );
        cv::Stitcher::Status status = stitcher->stitch(
            mats->toStdVector(), stitchedMat
        );

        if (status != cv::Stitcher::OK) {
            qDebug() << "ERROR: Cannot stitch images!";
            return;
        }

        tmpPix = matToPixmap(stitchedMat);
        stitchedMat.copyTo(currMat);
    } else if (mats->size() == 1) {
        tmpPix = matToPixmap(mats->first());
    }

    // Send picture to preview widget
    preview->setPixmap(tmpPix);
    preview->setVisible(true);
}

QPixmap MainWin::matToPixmap(cv::Mat mat)
{
    // Not my code!!! Put information about source here!!!

    if(mat.type() == CV_8UC1) {
        QImage img(
            static_cast<const uchar*>(mat.data), mat.cols, mat.rows,
            static_cast<int>(mat.step), QImage::Format_Indexed8
        );

        // Create and set color table
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        img.setColorTable(colorTable);

        return QPixmap::fromImage(img);
    } else if(mat.type()==CV_8UC3) {
        QImage img(
            static_cast<const uchar*>(mat.data), mat.cols, mat.rows,
            static_cast<int>(mat.step), QImage::Format_RGB888
        );

        return QPixmap::fromImage(img.rgbSwapped());
    } else {
        return QPixmap();
    }
}

void MainWin::about()
{
    QDialog about(this);
    Ui::AboutDialog uiAbout;
    uiAbout.setupUi(&about);
    about.setMinimumHeight(350);
    about.exec();
}

void MainWin::disconnectDevices()
{
    if (controllerConnected)
        connectController();
    if (cameraConnected)
        connectCamera();
}

void MainWin::reset()
{
    // Clear als mats
    currMat.release();

    // Reset stitched images object
    mats->clear();

    // Hide all previews and widgets
    preview->setVisible(false);
    previewLiveCamera->setVisible(false);

    // Disconnect camera and controller
    disconnectDevices();

    // Set guimode to normal
    guiMode = GuiMode::NORMAL;
}

void MainWin::saveImage()
{
    if (currMat.empty()) {
        QMessageBox::critical(
            this, tr("Save image"),
            tr("There is currently no image to save!")
        );
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save image"), QDir::homePath()
    );
    if (fileName.isEmpty())
        return;

    if (!cv::imwrite(fileName.toStdString(), currMat)) {
        QMessageBox::critical(
            this, tr("Save image"),
            tr("Could not save image!")
        );
    }
}

void MainWin::saveResults()
{
    // TODO: Save results
}

void MainWin::exit()
{
    close();
}

void MainWin::controllerReady()
{
    qDebug() << "Controller ready";
    if (guiMode == GuiMode::AUTOMATIC_CAMERA_STITCHING) {
        takeImage();
        if (!controller->hasReachedPosEnd())
            controller->moveToNextPos();
        else
            stitchImages();
    }
}

void MainWin::updatePreview()
{
    QPixmap pixTmp = matToPixmap(currMat);
    preview->setPixmap(pixTmp);
}

void MainWin::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);

    QSettings settings;
    settings.setValue("window_width", width());
    settings.setValue("window_height", height());
    settings.sync();
}

void MainWin::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open image file"), QDir::homePath()
    );
    if (fileName.isEmpty())
        return;

    cv::Mat matTmp = cv::imread(fileName.toStdString());
    QPixmap tmpPix;
    if (!tmpPix.load(fileName) || matTmp.data == nullptr) {
        QMessageBox::warning(
            this, tr("Cannot load image"),
            tr("Cannot load image from file name!")
        );
        return;
    }

    matTmp.copyTo(currMat);
    preview->setPixmap(tmpPix);
    addImagePathToRecent(fileName);
}

void MainWin::updateRecentMenu()
{
    // Clear all menu entries
    clearRecentMenu();

    // Add menu entries
    QSettings settings;
    QStringList recentImages = settings.value(
        "recent_images", QStringList()
    ).toStringList();
    createRecentMenu(recentImages);
}

void MainWin::addImagePathToRecent(QString &path)
{
    // Read the recent image paths
    QSettings settings;
    QStringList recentImages = settings.value(
        "recent_images", QStringList()
    ).toStringList();

    // If the path already exist remove it, else remove the last one
    if (recentImages.contains(path))
        recentImages.removeOne(path);
    else if (recentImages.size() > 5)
        recentImages.removeLast();

    // Add the new one and save
    recentImages.append(path);
    settings.setValue("recent_images", recentImages);
    settings.sync();

    updateRecentMenu();
}

void MainWin::clearRecentImages()
{
    QSettings settings;
    settings.setValue("recent_images", QStringList());
    settings.sync();

    updateRecentMenu();
}

void MainWin::openRecentImage()
{
    QAction *act = qobject_cast<QAction*>(sender());
    QString fileName = act->text();

    cv::Mat matTmp = cv::imread(fileName.toStdString());
    QPixmap tmpPix;
    if (!tmpPix.load(fileName) || matTmp.data == nullptr) {
        QMessageBox::warning(
            this, tr("Cannot load image"),
            tr("Cannot load image from file name!")
        );
        return;
    }

    matTmp.copyTo(currMat);
    preview->setVisible(true);
    preview->setPixmap(tmpPix);
    addImagePathToRecent(fileName);
}

void MainWin::createRecentMenu(QStringList &entries)
{
    for (int i = 0; i < entries.size(); i++) {
        QAction *act = new QAction(this);
        act->setText(entries.at(i));
        connect(act, &QAction::triggered, this, &MainWin::openRecentImage);
        ui.mRecent->addAction(act);
    }
}

void MainWin::clearRecentMenu()
{
    int numAct = ui.mRecent->actions().size();
    QList<QAction*> actions = ui.mRecent->actions();
    for (int i = 0; i < numAct; i++) {
        QAction *act = actions.at(i);
        if (act->text() == "Clear")
            continue;
        act->disconnect(this);
        ui.mRecent->removeAction(act);
        delete act;
    }
    ui.mRecent->clear();
}