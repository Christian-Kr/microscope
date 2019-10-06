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

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>

#include "ui_mainwin.h"
#include "ui_about.h"


// Forward declarations
class QThread;
class LiveCamera;
class ImagePreview;
class Controller;
class QGridLayout;
class QVBoxLayout;
class StitchingWidget;
class AutoStitchingStatus;

///
/// Enum class for declaration of different gui modes:
/// - NORMAL: Normal mode the application exist in
/// - AUTOMATIC_CAMERA_STITCHING: Automatic camera control is cativated. So
///   should be controlable till this process ends, except an stop button.
///
enum class GuiMode {
    NORMAL,
    AUTOMATIC_CAMERA_STITCHING
};

///
/// \brief Main window class of microscope
///
class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    ///
    /// \brief Get singleton object
    /// \return The singleton object. No one should every delete it!
    ///
    static MainWin* instance() {
        if (_instance == nullptr) {
            _instance = new MainWin();
        }
        return _instance;
    }

    ///
    /// \brief Destructor
    ///
    virtual ~MainWin() override;

    ///
    /// \brief Permit copy contructor on object (singleton)
    ///
    MainWin(const MainWin&) = delete;

    ///
    /// \brief Permit email operator on object (singelton)
    /// \return The new object reference
    ///
    MainWin& operator=(const MainWin&) = delete;

    ///
    /// \brief Convert an opencv mat array to qpixmap
    /// This function has not been programmed by myself! This one should be
    /// static cause it could be a helpful function for many points in this
    /// application.
    /// \param mat OpenCV mat array
    ///
    static QPixmap matToPixmap(cv::Mat mat);

    ///
    /// \brief Read all settings for the application
    ///
    void readSettings();

public slots:
    ///
    /// \brief Clear recent images list
    ///
    void clearRecentImages();

    ///
    /// \brief Run the process of camera stitching
    ///
    void stitchImages();

    ///
    /// \brief Run automatic image stitching
    /// This function will control the motors over the arduino device to move
    /// the microscope one step and take an image. This procedure will be
    /// repeated till the distance limits are reached.
    ///
    void runAutoCameraStitching();

    ///
    /// Stop the camera stitching process
    ///
    void abortCameraStitching();

    ///
    /// Abort auto camera stitching process
    ///
    void abortAutoCameraStitching();

    ///
    /// \brief Take the image from the camera and make a stiched preview
    ///
    void takeImageFromCamera();

    ///
    /// \brief Save all images that are in the stitching widget
    ///
    void saveAllImages();

    ///
    /// \brief Save the selected image
    ///
    void saveSelectedImage();

    ///
    /// \brief Delete the current selected image
    ///
    void deleteImage();

    /**
     * Show the about dialog
     */
    void about();

    /**
     * Connect the camera modul with opencv
     */
    void connectCamera();

    /**
     * Connect the microscope controller modul
     */
    void connectController();

    /**
     * Connect all devices
     */
    void connectDevices();

    /**
     * Disconnect all devices
     */
    void disconnectDevices();

    /**
     * Reset the whole application
     */
    void reset();

    /**
     * Exit the application
     */
    void exit();

    /**
     * Save the image
     */
    void saveImage();

    /**
     * Controller is ready for doing more
     */
    void controllerReady();

    /**
     * Open an image
     */
    void openImage();

    /**
     * Open a recent image from menu click
     */
    void openRecentImage();

    /**
     * Update the preview from the opencv mat
     */
    void updatePreview();

    ///
    /// \brief Stop the auto scanning process
    ///
    void stopAutoScanningProcess();

signals:
    /**
     * Run camera
     * @param cap Video capture device from OpenCV
     */
    void runLiveCamera();

protected:
    ///
    /// \brief Override close event from QMainWindow
    ///
    virtual void closeEvent(QCloseEvent *event) override;

    ///
    /// \brief Override resize event from QMainWindow
    ///
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    ///
    /// \brief Build some connections for signol slot system
    ///
    void buildConnections();

    ///
    /// \brief Update the recent menu
    ///
    void updateRecentMenu();

    ///
    /// \brief Add given path of image to recent
    /// \param path Image file name
    ///
    void addImagePathToRecent(QString &path);

    /**
     * Create recent menu from list
     * @param entries List of entries to set
     */
    void createRecentMenu(QStringList &entries);

    /**
     * Clear recent menu
     */
    void clearRecentMenu();

    /**
     * Constructor
     * @param parent Parent window pointer
     */
    MainWin(QWidget* parent = nullptr, Qt::WindowFlags flags = nullptr);

    /**
     * Initialize the camera from the microscope
     * @return True if camera connected and open, else false
     */
    bool initCamera();

    /**
     * Initialize the microscope controller
     * @return True if controller available and connected, else false
     */
    bool initController();

    /**
     * Live image from camera has been updated
     */
    void liveImageUpdated();

    /**
     * Live camera has been exit
     */
    void liveCameraExit();

private:
    cv::Mat currMat;
    QVector<cv::Mat> *mats;
    cv::VideoCapture *cap;

    static MainWin* _instance;

    Ui::MainWin ui;

    bool cameraConnected;
    bool controllerConnected;
    bool stopAutoScanning;

    QThread *thread;
    QLabel *labelStatusCamera;
    QLabel *labelStatusController;

    LiveCamera* liveCamera;
    GuiMode guiMode;

    ImagePreview *preview;
    ImagePreview *previewLiveCamera;
    QVBoxLayout *layoutMain;
    QGridLayout *layoutImages;

    Controller *controller;

    int gridNumMaxX;
    int gridNumMaxY;

    StitchingWidget * stitchWidget;
    AutoStitchingStatus * statusWidget;
};


#endif // MAINWIN_H
