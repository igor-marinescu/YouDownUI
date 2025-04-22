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

#include <QDateTime>
#include <QDir>

#include <stdlib.h> // getenv()

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "editlinetext.h"
#include "add_edit.h"

#if (defined (_WIN32) || defined (_WIN64))
//******************************************************************************
// Windows: "%APPDATA%\APP_NAME\"
//******************************************************************************
QString get_config_location_win(QString filename)
{
    QString config_dir_name;
    char * ptr_str;
    size_t len;
    errno_t err = _dupenv_s(&ptr_str, &len, "appdata");
    if(!err)
    {
        config_dir_name = QString(ptr_str) + QDir::separator() + QString(APP_MAME);
        free(ptr_str);
        
        if(!QDir(config_dir_name).exists())
        {
            if(!QDir().mkdir(config_dir_name))
            {
                // Failed to create directory, store the settings along with the binary file.
                return filename;
            }
        }
        
        // Directory exists (or successfully created), return file name
        return (config_dir_name + QDir::separator() + filename);
    }

    // %APPDATA% not defined, the settings are stored along with the binary file.
    return filename;
}
#endif

//******************************************************************************
// Linux: "$HOME/.config/APP_MAME/" or "$XDG_CONFIG_HOME/APP_NAME/"
//          
// `$XDG_CONFIG_HOME` defines the base directory relative to which user-specific 
// configuration files should be stored. If `$XDG_CONFIG_HOME` is either not set 
// or empty, a default equal to `$HOME/.config` should be used.
//******************************************************************************
#if (defined (LINUX) || defined (__linux__))
QString get_config_location_linux(QString filename)
{
    QString config_dir_name;
    char * ptr_str = getenv("XDG_CONFIG_HOME");
    if(ptr_str)
    {
        config_dir_name = QString(ptr_str);
    }
    else{
        ptr_str = getenv("HOME");
        if(ptr_str)
        {
            config_dir_name = QString(ptr_str) + QDir::separator() + QString(".config");
        }
    }

    // If $XDG_CONFIG_HOME or $HOME defined, check if settings directory 
    // APP_MAME already exists, if not create it.
    if(ptr_str)
    {
        config_dir_name += QDir::separator() + QString(APP_MAME);
        if(!QDir(config_dir_name).exists())
        {
            if(!QDir().mkdir(config_dir_name))
            {
                // Failed to create directory, store the settings along with the binary file.
                return filename;
            }
        }
         
        // Directory exists (or successfully created), return file name
        return (config_dir_name + QDir::separator() + filename);
    }

    // If either $XDG_CONFIG_HOME or $HOME defined, the settings are stored 
    // along with the binary file.
    return filename;
}
#endif

//******************************************************************************
QString get_config_location(QString filename)
{    
#if (defined (_WIN32) || defined (_WIN64))
    return get_config_location_win(filename);
#elif (defined (LINUX) || defined (__linux__))
    return get_config_location_linux(filename);
#else
    return filename;
#endif
}

//******************************************************************************
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create application interface
    appInterface = new AppInterface(get_config_location(QString("config.cfg")), this);
    appInterface->loadWidgetGeometry(this, "WinPos");

    // Connect signals
    connect(&dfu, SIGNAL(sigFinished(int)), this, SLOT(dfu_finished(int)));
    dfu.output = ui->out;

    // Load settings

    // Downloader Executable
    settingsData.downloaderExe = appInterface->confFile.get_qstr("Settings", "DownloaderExe");

    // Output Directory
    settingsData.outDir = appInterface->confFile.get_qstr("Settings", "OutputDir");

    // Queue File/Dir
    settingsData.queueFileDir = appInterface->confFile.get_qstr("Settings", "QueueFileDir");

    // Default Filters
    char defFilterName[32] = "DefFilter_";
    QString defFilterVal;
    for(char i = 'A'; i < 'N'; i++){
        defFilterName[9] = i;
        defFilterVal = appInterface->confFile.get_qstr("Settings", defFilterName);
        defFilterVal = defFilterVal.trimmed();
        if(!defFilterVal.isEmpty()){
            settingsData.defFormats << defFilterVal;
        }
    }

    settingsData.preprocessLink = (appInterface->confFile.get("Settings", "PreprocessLinks") != 0 ? true : false);
    settingsData.appendTimestamp = (appInterface->confFile.get("Settings", "AppendTimestamp") != 0 ? true : false);
    settingsData.setModifiedTime = (appInterface->confFile.get("Settings", "SetModifiedTime") != 0 ? true : false);

    // Background color for output
    QPalette p = ui->out->palette(); // define pallete for textEdit..
    p.setColor(QPalette::Base, Qt::black); // set color "Red" for textedit base
    p.setColor(QPalette::Text, Qt::green);
    ui->out->setPalette(p); // change textedit palette

    // Queue
    queue.assignTab(ui->tableWidget);
    queue.resizeTab(width());
    queue.load(settingsData.queueFileDir);

    elLastDownFlag = false;

    out("********************************************************************");
    out("[Start] " + QString(APP_MAME) + " " + QString(APP_VERS) + " Built: " + QString(__DATE__) + " " + QString(__TIME__));
    out("********************************************************************");

    this->setWindowTitle(QString(APP_MAME) + " " + QString(APP_VERS));
}

//******************************************************************************
MainWindow::~MainWindow()
{
    queue.save(settingsData.queueFileDir);
    queue.releaseTab();

    // Save settings

    // Downloader Executable
    appInterface->confFile.set_qstr("Settings", "DownloaderExe", settingsData.downloaderExe);

    // Output Directory
    appInterface->confFile.set_qstr("Settings", "OutputDir", settingsData.outDir);

    // Queue File/Dir
    appInterface->confFile.set_qstr("Settings", "QueueFileDir", settingsData.queueFileDir);

    // Default Filters
    char defFilterName[32] = "DefFilter_";
    char defFilterIdx = 'A';
    QString defFilterVal;
    for(int i = 0; i < settingsData.defFormats.size(); ++i){
        defFilterVal = settingsData.defFormats.at(i);
        defFilterVal = defFilterVal.trimmed();
        if(!defFilterVal.isEmpty() && (defFilterIdx < 'N')){
            defFilterName[9] = defFilterIdx;
            appInterface->confFile.set_qstr("Settings", defFilterName, defFilterVal);
            defFilterIdx++;
        }
    }

    appInterface->confFile.set("Settings", "PreprocessLinks", settingsData.preprocessLink ? true : false);
    appInterface->confFile.set("Settings", "AppendTimestamp", settingsData.appendTimestamp ? true : false);
    appInterface->confFile.set("Settings", "SetModifiedTime", settingsData.setModifiedTime ? true : false);

    // Clear old unused Default Filters
    for(; defFilterIdx < 'N'; defFilterIdx++){
        defFilterName[9] = defFilterIdx;
        appInterface->confFile.set_qstr("Settings", defFilterName, "");
    }

    // Destroy application interface
    appInterface->saveWidgetGeometry(this, "WinPos");
    delete appInterface;

    out("[End]");
    delete ui;
}

//******************************************************************************
// Enable/Disable UI control elements (which starts the download process)
//******************************************************************************
void MainWindow::enableElements(bool state)
{
    ui->btnDownload->setEnabled(state);
    ui->btnUpdate->setEnabled(state);
    ui->btnListFormats->setEnabled(state);
}

//******************************************************************************
void MainWindow::out(const QString & txt)
{
    ui->out->append(txt);
    log.out(txt);
}

//******************************************************************************
bool MainWindow::showError(const QString & text, const QString & textExt)
{
    out("[Error] " + text + " " + textExt);
    AppInterface::MsgErr(text, textExt);
    return false;
}

//******************************************************************************
// Settings
//******************************************************************************
void MainWindow::on_btnSettings_clicked()
{
    // Show settings dialog
    Settings settings(this, &settingsData);

    settings.setModal(true);
    settings.exec();

    // If settings changed
    if(settings.result() == QDialog::Accepted){
        // Rebuild Filters List
        QString defFilterVal;
        for(int i = 0; i < settingsData.defFormats.size(); ++i){
            defFilterVal = settingsData.defFormats.at(i);
            defFilterVal = defFilterVal.trimmed();
        }
    }
}

//******************************************************************************
void MainWindow::dfu_finished(int exitCode)
{
    out("Process finished (" + QString::number(exitCode) + ")");
    out("--------------------------------------------------------------------");

    // In case if the process failed
    if(exitCode != 0)
    {
        elLastDownFlag = false;
        showError("Process finished with error: " + QString::number(exitCode),
                  "Check logs, correct data and try again");
        enableElements(true);
        return;
    }

    // In case if this process is finished for downloading an element
    if(elLastDownFlag)
    {
        elLastDownFlag = false;
        // remove it from queue
        Queue::Element element;
        if(queue.get(0, &element))
        {
            // it is the same element we just downloaded?
            if(Queue::elementsEqual(element, elLastDown))
                // remove it from queue
                queue.del(0);
        }
        // Start automatically downloading next link
        enableElements(!downloadNext());
    }
}

//******************************************************************************
// Execute Command
//******************************************************************************
bool MainWindow::execute(const QStringList & arguments, const QString & txt)
{
    out("");
    out("--------------------------------------------------------------------");
    out(txt);

    // Check if process is already running
    if(dfu.isBusy())
        return showError("Process already running", "please wait...");

    // Try to start the process
    if(settingsData.downloaderExe.isEmpty())
        return showError("Downloader executable not defined", "");

    if(!dfu.execProcess(settingsData.downloaderExe, arguments))
        return showError("Can not start process", "");

    out("Process started, please wait...");
    return true;
}

//******************************************************************************
// Download Next from list
// return true - if download started, else false
//******************************************************************************
bool MainWindow::downloadNext()
{
    if(dfu.isBusy())
        return false;

    // get top element from queue (if exist)
    if(!queue.get(0, &elLastDown))
        return false;

    // start building arguments
    QStringList arguments;

    // Video?
    if(!elLastDown.audio)
    {
        // add format?
        if(!elLastDown.format.isEmpty())
        {
            arguments << "-f" << elLastDown.format;
        }
    }
    // Audio?
    else{
        arguments << "--extract-audio" << "--audio-format" << "mp3";
    }

    // Set File Modification Time
    if(settingsData.setModifiedTime)
    {
        arguments << "--mtime";
    }    
    else{
        arguments << "--no-mtime";
    }
    
    // Set output filename format "-o <filename>"
    QString timestamp = "";
    
    if(settingsData.appendTimestamp)
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        timestamp = dateTime.toString("-hhmmss_ddMMyy");
    }

    QString output_filename = "%(title)s" + timestamp + ".%(ext)s";
    
    // add destination path?
    if(!settingsData.outDir.isEmpty())
    {
        output_filename = settingsData.outDir + QDir::separator() + output_filename;
    }

    arguments << "-o" << output_filename;
    
    // add link
    // preprocess link?
    QString strLink = elLastDown.link;
    if(settingsData.preprocessLink)
    {
        strLink = preProcessLink(strLink);
        out("Preprocessed link:" + strLink);
    }
    arguments << strLink;
    elLastDownFlag = execute(arguments, ("Download: " + Queue::strElement(&elLastDown)));

    return elLastDownFlag;
}

//******************************************************************************
// Add (add element to list)
//******************************************************************************
void MainWindow::add(bool audio)
{
    Queue::Element element;
    element.audio = audio;
    AddEdit add(this, &settingsData, &element);

    add.setModal(true);
    add.exec();

    if(add.result() == QDialog::Accepted)
    {
        // add element to queue
        if(queue.add(element))
        {
            out("Added: " + Queue::strElement(&element));

            // start automatically downloading next
            if(!dfu.isBusy())
            {
                enableElements(!downloadNext());
            }
        }
        else
        {
            showError("Cannot add element", "Inconsistent data");
        }
    }
}

//******************************************************************************
void MainWindow::on_btnAudio_clicked()
{
    add(true);
}

void MainWindow::on_btnVideo_clicked()
{
    add(false);
}

//******************************************************************************
// Download
//******************************************************************************
void MainWindow::on_btnDownload_clicked()
{
    // if not busy start downloading next from queue
    if(!dfu.isBusy())
    {
        // start with dowloading next from list,
        enableElements(!downloadNext());
    }
}

//******************************************************************************
// Edit (edit an element from list)
//******************************************************************************
void MainWindow::on_btnEdit_clicked()
{
    Queue::Element element;
    int idx = ui->tableWidget->currentRow();
    if(queue.get(idx, &element))
    {
        AddEdit edit(this, &settingsData, &element);

        edit.setModal(true);
        edit.exec();

        if(edit.result() == QDialog::Accepted)
        {
            // edit element in list
            if(queue.edit(idx, element))
            {
                out("Edited: " + Queue::strElement(&element));
            }
            else
            {
                showError("Cannot edit element", "Inconsistent data");
            }
        }
    }
    else
    {
        showError("Cannot edit element", "No element selected");
    }
}

//******************************************************************************
// Remove (remove element from list)
//******************************************************************************
void MainWindow::on_btnRemove_clicked()
{
    if(!queue.del(ui->tableWidget->currentRow()))
    {
        showError("Cannot remove element", "No element selected");
    }
}

//******************************************************************************
// Clear list (remove all elements from list)
//******************************************************************************
void MainWindow::on_btnClear_clicked()
{
    queue.clear();
}

//******************************************************************************
// List Available Formats
//******************************************************************************
void MainWindow::on_btnListFormats_clicked()
{
    QString text;
    EditLineText editLink(this, "Link", &text);

    editLink.setModal(true);
    editLink.exec();

    if((editLink.result() == QDialog::Accepted) && !text.isEmpty())
    {
        QStringList arguments;
        arguments << "-F" << text;
        execute(arguments, "List formats");
    }
}

//******************************************************************************
// Update
//******************************************************************************
void MainWindow::on_btnUpdate_clicked()
{
    QStringList arguments;
    arguments << "-U";
    execute(arguments, "Update");
}

//******************************************************************************
// Preprocess Link
//******************************************************************************
QString MainWindow::preProcessLink(QString strLink)
{
    QString strFinal;

    // Split by '?'
    // Ex: https://www.youtube.com/watch?v=wTQxiGJzR_I&list=RDKQu8FOjJXdI&index=3
    //  0: https://www.youtube.com/watch
    //  1: v=wTQxiGJzR_I&list=RDKQu8FOjJXdI&index=3
    QStringList strList = strLink.split(QChar('?'));
    if(strList.size() >= 2){

        // Add element 0 to final:
        strFinal = strList.at(0);

        // Apply processing only for youtube links
        if(!strFinal.contains("youtube.", Qt::CaseInsensitive)
        && !strFinal.contains("youtu.be", Qt::CaseInsensitive))
        {
            return strLink;
        }

        // Split 1 from previous list by '&'
        // Ex: v=wTQxiGJzR_I&list=RDKQu8FOjJXdI&index=3
        //  0: v=wTQxiGJzR_I
        //  1: list=RDKQu8FOjJXdI
        //  3: index=3
        QStringList strLst2 = strList.at(1).split(QChar('&'));

        // Parse elements
        for(int i = 0; i < strLst2.size(); i++){

            // Split every element from previous list by '='
            QStringList strLst3 = strLst2.at(i).split(QChar('='));

            // Acce pt only pairs (0 and 1)
            if(strLst3.size() == 2){

                // Accept only if element 0 is 'v'
                if((strLst3.at(0) == "v") || (strLst3.at(0) == "V")){

                    // Add to final
                    strFinal += "?" + strLst3.at(0) + "=" + strLst3.at(1);
                    break;
                }
            }
        }
    }
    else{
        // Cannot split, just return initial string
        return strLink;
    }

    return strFinal;
}
