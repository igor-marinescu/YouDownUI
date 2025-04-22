/*
 * This file is part of the YouDownUI distribution.
 * Copyright (c) 2020 Igor Marinescu (igor.marinescu@gmail.com).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
//******************************************************************************
// Application Interface
//******************************************************************************
#ifndef APPINTERFACE_H
#define APPINTERFACE_H

//******************************************************************************
// Includes
//******************************************************************************
#include "ConfFile.h"
#include <QWidget>
#include <QMessageBox>

//******************************************************************************
// Class
//******************************************************************************
class AppInterface{

    QWidget * mainWindow;
    bool in_loadWidgetGeometry;
    QWidget * wid_loadWidgetGeometry;

public:

    //--------------------------------------------------------------------------
    class Settings {
    public:

/*
        Settings& Settings::operator=(const Settings& s) {
            return *this;
        }
*/
    } settings;
    //--------------------------------------------------------------------------

    ConfFile confFile;

    AppInterface(QString filename, QWidget * mWindow);
    AppInterface(const char * filename, QWidget * mWindow);
    ~AppInterface();

    void saveWidgetGeometry(QWidget * wid, const char * category);
    void loadWidgetGeometry(QWidget * wid, const char * category);

    // Messages
    static int MsgErr(const QString & txt, const QString & info);
    static int MsgWrn(const QString & txt, const QString & info,
                QMessageBox::StandardButtons btns = QMessageBox::Ok,
                QMessageBox::StandardButton btn = QMessageBox::Ok);

    // Open/Save dialogs
    QString SaveDialog(const QString & directory,
                        const QString & filter,
                       const QString & extension);
    QString OpenDialog(const QString & directory,
                        const QString & filter,
                       const QString & extension);
};

//******************************************************************************
#endif // APPINTERFACE_H
