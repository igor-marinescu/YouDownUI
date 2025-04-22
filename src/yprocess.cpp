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
// YouDown Batch Process
//
// Igor Marinescu
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include "yprocess.h"

//******************************************************************************
// Constructor
//******************************************************************************
YProcess::YProcess(QObject *parent) :
    flagStarted(false),
    flagFinished(false),
    flagBusy(false),
    output(nullptr){

    (void) parent;

    // Connect QProcess signals to local slots
    connect(this, SIGNAL(readyReadStandardOutput()),
        this, SLOT(logOutput()));

    connect(this, SIGNAL(readyReadStandardError()),
        this, SLOT(logErrorOutput()));

    connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(logFinished(int, QProcess::ExitStatus)));
}

//******************************************************************************
// Destructor
//******************************************************************************
YProcess::~YProcess(){

    // Terminate the process if not finished by this time
    if(!flagFinished)
        terminate();
}

//******************************************************************************
bool YProcess::isBusy()
{
    return flagBusy;
}

//******************************************************************************
// Catch the output stream event and redirect to UI
//******************************************************************************
void YProcess::logOutput(){

    QByteArray byteArray = readAllStandardOutput();
    QStringList lines = QString(byteArray).split("\n");
    foreach(QString line, lines){
        line.remove(QChar('\r'), Qt::CaseInsensitive);
        if(output && !line.isNull() && !line.isEmpty())
            output->append(line);
        emit sigOutput(line);
    }
}

//******************************************************************************
// Catch the error output stream event and redirect to UI
//******************************************************************************
void YProcess::logErrorOutput(){

    QByteArray byteArray = readAllStandardError();
    QStringList lines = QString(byteArray).split("\n");
    foreach(QString line, lines){
        line.remove(QChar('\r'), Qt::CaseInsensitive);
        if(output)
            output->append(line);
        emit sigErrorOutput(line);
    }
}

//******************************************************************************
// Catch finished signal and notify UI
//******************************************************************************
void YProcess::logFinished(int exitCode, QProcess::ExitStatus exitStatus){

    (void) exitStatus;

    flagFinished = true;
    flagStarted = false;
    flagBusy = false;

    emit sigFinished(exitCode);
}

//******************************************************************************
// Execute external process
//******************************************************************************
bool YProcess::execProcess(const QString & cmd, const QStringList & args)
{

    flagStarted = false;
    flagFinished = false;

    QString program = cmd;

    QString exeLine = program;
    foreach(QString line, args){ exeLine += " " + line;}
    if(output)
        output->append(exeLine);

    // Start
    start(program, args, QProcess::Unbuffered | QProcess::ReadWrite);
    flagStarted = waitForStarted();
    flagBusy = flagStarted;

    return flagStarted;
}
