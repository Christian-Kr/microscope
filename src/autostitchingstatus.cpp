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

#include <QtGui/QCloseEvent>

#include "autostitchingstatus.hpp"


AutoStitchingStatus::AutoStitchingStatus(
    QString label, QWidget *parent, bool closeable)
    : QWidget(parent),
    closeable(closeable)
{
    ui.setupUi(this);
    setLabel(label);
}

AutoStitchingStatus::~AutoStitchingStatus()
{
}

void AutoStitchingStatus::setLabel(QString label)
{
    ui.labelStatus->setText(label);
}

void AutoStitchingStatus::closeEvent(QCloseEvent * event)
{
    // Do nothing! Tis window should not be closeable by the window system!
    if (!closeable)
        event->ignore();
}

void AutoStitchingStatus::stopPressed()
{
    emit stopAutoScanning();
}
