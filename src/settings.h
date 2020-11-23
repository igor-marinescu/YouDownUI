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
// Settings window
//******************************************************************************
#ifndef SETTINGS_H
#define SETTINGS_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QDialog>

//******************************************************************************
namespace Ui {
class Settings;
}

//******************************************************************************
class Settings : public QDialog
{
    Q_OBJECT

public:

    typedef struct {
        QString outDir;
        QString queueFileDir;
        QStringList defFormats;
        bool preprocessLink;
    }  SettingsData;


    explicit Settings(QWidget *parent = 0, SettingsData * settingsData = nullptr);
    ~Settings();

private slots:
    void on_buttonBox_accepted();

    void on_btnBrowseQueue_clicked();

    void on_btnBrowseOutput_clicked();

private:
    Ui::Settings *ui;
    SettingsData * settingsDataPtr;
};

//******************************************************************************
#endif // SETTINGS_H
