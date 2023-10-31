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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "editlinetext.h"

//******************************************************************************
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create application interface
    appInterface = new AppInterface("config.cfg", this);
    appInterface->loadWidgetGeometry(this, "WinPos");

    //--------------------------------------------------------------------------
    // Connect signals
    //--------------------------------------------------------------------------
    connect(&dfu, SIGNAL(sigFinished(int)), this, SLOT(dfu_finished(int)));
    dfu.output = ui->out;

    //--------------------------------------------------------------------------
    // Load settings
    //--------------------------------------------------------------------------
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
    // Preprocess Links
    settingsData.preprocessLink = (appInterface->confFile.get("Settings", "PreprocessLinks") != 0 ? true : false);

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

    //--------------------------------------------------------------------------
    // Save settings
    //--------------------------------------------------------------------------
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
    // Preprocess Links
    appInterface->confFile.set("Settings", "PreprocessLinks", settingsData.preprocessLink ? true : false);

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
bool MainWindow::setError(const QString & text, const QString & textExt, Error * err)
{
    if(err != nullptr)
    {
        err->code = -1;
        err->text = text;
        err->textExt = textExt;
    }
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
    enableElements(true);

    // in case if the process failed
    if(exitCode != 0)
    {
        // uncheck download button, do not start automatically downloading next
        ui->btnDownload->setChecked(false);
        elLastDownFlag = false;

        setError("Process finished with error: " + QString::number(exitCode),
                 "Check logs, correct data and try again");
    }

    // in case if this process is finished for downloading an element
    // remove it from list
    if(elLastDownFlag){
        elLastDownFlag = false;
        // remove it from queue
        Queue::Element element;
        if(queue.get(0, &element)){
            // it is the same element we just downloaded?
            if(Queue::elementsEqual(element, elLastDown))
            {
                // remove it from queue
                queue.del(0);
            }
        }
    }

    // if download button checked, start downloading next element
    if(ui->btnDownload->isChecked())
    {
        Error error;
        ui->btnDownload->setChecked(downloadNext(&error));
    }
}

//******************************************************************************
// Execute Command
//******************************************************************************
bool MainWindow::execute(const QStringList & arguments, const QString & txt, Error * error)
{
    out("");
    out("--------------------------------------------------------------------");
    out(txt);

    // Check if process is already running
    if(dfu.flagStarted)
    {
        return setError("Process already running", "please wait...", error);
    }

    // Try to start the process
    if(settingsData.downloaderExe.isEmpty())
    {
        return setError("Downloader executable not defined", "", error);
    }
    if(!dfu.execProcess(settingsData.downloaderExe, arguments))
    {
        return setError("Can not start process", "", error);
    }

    out("Process started, please wait...");
    enableElements(false);
    return true;
}

//******************************************************************************
// Download Next from list
// return true - if download started, else false
//******************************************************************************
bool MainWindow::downloadNext(Error * error)
{
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
        // add arguments
        arguments << "--extract-audio" << "--audio-format" << "mp3";
    }

    // add destination path?
    if(!settingsData.outDir.isEmpty())
    {
        QString outDir = settingsData.outDir + "\\%(title)s.%(ext)s";
        arguments << "-o" << outDir;
    }

    // add link
    // preprocess link?
    QString strLink = elLastDown.link;
    if(settingsData.preprocessLink)
    {
        strLink = preProcessLink(strLink);
        out("Preprocessed link:" + strLink);
    }
    arguments << strLink;
    elLastDownFlag = execute(arguments, ("Download: " + Queue::strElement(&elLastDown)), error);

    return elLastDownFlag;
}

//******************************************************************************
// Download
//******************************************************************************
void MainWindow::on_btnDownload_clicked()
{
    // if download button checked
    if(ui->btnDownload->isChecked())
    {
        // if not busy start downloading next from queue
        if(!dfu.flagBusy)
        {
            Error error;
            // start with dowloading next from list,
            // uncheck button in case if list empty or error
            ui->btnDownload->setChecked(downloadNext(&error));
        }
    }
}

//******************************************************************************
// Add (add element to list)
//******************************************************************************
void MainWindow::on_btnAdd_clicked()
{
    Queue::Element element;
    element.audio = false;
    AddEdit add(this, &settingsData, &element);

    add.setModal(true);
    add.exec();

    if(add.result() == QDialog::Accepted)
    {
        // add element to queue
        if(queue.add(element))
        {
            out("Added: " + Queue::strElement(&element));

            //!!! todo: start automatically download next?
            //on_btnDownload_clicked()
        }
        else
        {
            setError("Cannot add element", "Inconsistent data");
        }

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
                setError("Cannot edit element", "Inconsistent data");
            }
        }
    }
    else
    {
        setError("Cannot edit element", "No element selected");
    }
}

//******************************************************************************
// Remove (remove element from list)
//******************************************************************************
void MainWindow::on_btnRemove_clicked()
{
    if(!queue.del(ui->tableWidget->currentRow()))
    {
        setError("Cannot remove element", "No element selected");
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
