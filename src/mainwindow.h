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
// Main Window
//******************************************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//******************************************************************************
//******************************************************************************
#include <QMainWindow>

#include "AppInterface.h"
#include "settings.h"
#include "yprocess.h"
#include "logging.h"
#include "queue.h"

//******************************************************************************
// Version
//******************************************************************************

//******************************************************************************
namespace Ui {
class MainWindow;
}

//******************************************************************************
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void dfu_finished(int exitCode);
    void on_btnListFormats_clicked();
    void on_btnSettings_clicked();
    void on_btnUpdate_clicked();
    void on_btnRemove_clicked();
    void on_btnClear_clicked();
    void on_btnEdit_clicked();
    void on_btnDownload_clicked();
    void on_btnAudio_clicked();
    void on_btnVideo_clicked();

private:
    Ui::MainWindow *ui;
    AppInterface * appInterface;
    YProcess dfu;

    Settings::SettingsData settingsData;

    void out(const QString & txt);
    bool execute(const QStringList & arguments, const QString & txt);
    bool downloadNext();

    void enableElements(bool state);

    QString preProcessLink(QString strLink);

    Logging log;
    Queue queue;

    // Last downloaded element
    Queue::Element elLastDown;
    bool elLastDownFlag;

    bool showError(const QString & text, const QString & textExt = "");

    void add(bool audio);
};

//******************************************************************************
#endif // MAINWINDOW_H
