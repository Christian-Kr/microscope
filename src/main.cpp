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

#include <QtWidgets/QApplication>
#include <opencv2/opencv.hpp>

#include "mainwin.hpp"

///
/// \brief Main entry point of application microscope
/// \param argc Number of arguments
/// \param argv List of arguments
/// \return Zero if exit success, else -1
///
int main(int argc, char* argv[])
{
    // Creating main application object
    QApplication *app = new QApplication(argc, argv);

    // Setting these informations is essential for QSettings instance. This
    // makes calling it easier from the whole application.
    app->setOrganizationName("Krippendorf");
    app->setApplicationName("Microscope");
    app->setApplicationVersion("1.0.0");

    // Creating the main window object. This one is a singleton, having the
    // same instance in the whole application.
    MainWin* mainWin = MainWin::instance();
    mainWin->setVisible(true);

    // Run the event loop
    return app->exec();
}
