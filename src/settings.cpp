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

//******************************************************************************
// Includes
//******************************************************************************
#include "settings.h"
#include "ui_settings.h"
#include "QFileDialog"

//******************************************************************************
Settings::Settings(QWidget *parent, SettingsData * settingsData) :
    QDialog(parent),
    ui(new Ui::Settings),
    settingsDataPtr(nullptr)
{
    ui->setupUi(this);

    settingsDataPtr = settingsData;
    if(settingsDataPtr != nullptr){

        ui->edtDownloaderExe->setText(settingsDataPtr->downloaderExe);
        ui->edtOutputDir->setText(settingsDataPtr->outDir);
        ui->edtQueueFileDir->setText(settingsDataPtr->queueFileDir);
        for(int i = 0; i < settingsDataPtr->defFormats.size(); ++i)
            ui->txtDefFormats->append(settingsDataPtr->defFormats.at(i));
        ui->chkboxPreprocessLinks->setChecked(settingsDataPtr->preprocessLink);
        ui->chkboxAppendTimestamp->setChecked(settingsDataPtr->appendTimestamp);
        ui->chkboxSetModifiedTime->setChecked(settingsDataPtr->setModifiedTime);
    }
}

//******************************************************************************
Settings::~Settings()
{
    delete ui;
}

//******************************************************************************
void Settings::on_buttonBox_accepted()
{
    if(settingsDataPtr != nullptr){
        settingsDataPtr->downloaderExe = ui->edtDownloaderExe->text();
        settingsDataPtr->outDir = ui->edtOutputDir->text();
        settingsDataPtr->queueFileDir = ui->edtQueueFileDir->text();
        settingsDataPtr->defFormats = ui->txtDefFormats->toPlainText().split('\n');
        settingsDataPtr->preprocessLink = ui->chkboxPreprocessLinks->isChecked();
        settingsDataPtr->appendTimestamp = ui->chkboxAppendTimestamp->isChecked();
        settingsDataPtr->setModifiedTime = ui->chkboxSetModifiedTime->isChecked();
    }
}

//******************************************************************************
void Settings::on_btnBrowseQueue_clicked()
{
#if (defined (_WIN32) || defined (_WIN64))
    const char * dialog_filter = "Any File (*.*)";
#else
    const char * dialog_filter = "Any File (*)";
#endif
    
    QFileDialog dialog(this,
                        "Queue File",
                       settingsDataPtr->queueFileDir,
                       dialog_filter);

    //dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if(dialog.exec()){
        QStringList fileNames = dialog.selectedFiles();
        ui->edtQueueFileDir->setText(fileNames[0]);
    }
}

//******************************************************************************
void Settings::on_btnBrowseOutput_clicked()
{
    QFileDialog dialog(this,
                        "Output Directory",
                       settingsDataPtr->outDir);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if(dialog.exec()){
        QStringList fileNames = dialog.selectedFiles();
        ui->edtOutputDir->setText(fileNames[0]);
    }
}

//******************************************************************************
void Settings::on_btnDownloaderExe_clicked()
{
#if (defined (_WIN32) || defined (_WIN64))
    const char * dialog_filter = "Exe File (*.exe)";
#else
    const char * dialog_filter = "Exe File (*)";
#endif

    QFileDialog dialog(this,
                        "Executable File",
                       settingsDataPtr->downloaderExe,
                       dialog_filter);

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if(dialog.exec()){
        QStringList fileNames = dialog.selectedFiles();
        ui->edtDownloaderExe->setText(fileNames[0]);
    }
}
