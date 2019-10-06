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

#ifndef AUTOSTITCHINGSTATUS_H
#define AUTOSTITCHINGSTATUS_H

#include <QtWidgets/QWidget>

#include "ui_status.h"


///
/// \brief Information dialog for auto stitching
///
class AutoStitchingStatus : public  QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Overloaded constructor
    /// \param parent Parent widget
    /// \param label Label of the widget
    ///
    explicit AutoStitchingStatus(
        QString label, QWidget *parent = nullptr, bool closeable = true);

    ///
    /// \brief Destructor
    ///
    virtual ~AutoStitchingStatus() override;

    ///
    /// \brief Set label to draw
    ///
    void setLabel(QString label);

    ///
    /// \brief Set progress bar information
    /// \param max Maximum value
    /// \param current Current value
    ///
    void setProgressInformation(int max, int curr);

public slots:
    ///
    /// \brief Stop has been pressed
    ///
    void stopPressed();

signals:
    ///
    /// \brief Emited when stop has been pressed and the process should be
    /// aborted
    ///
    void stopAutoScanning();

protected:
    ///
    /// \brief Override the close event method
    /// \param event The close event
    ///
    virtual void closeEvent(QCloseEvent *) override;

private:
    bool closeable;

    Ui::statusWidget ui;
};


#endif // AUTOSTITCHINGSTATUS_H
