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
#ifndef YPROCESS_H
#define YPROCESS_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QObject>
#include <QProcess>
#include <QTextEdit>

//******************************************************************************
// Includes
//******************************************************************************
class YProcess : public QProcess {
    Q_OBJECT

public:

    YProcess(QObject *parent = 0);
    ~YProcess();

    bool execProcess(const QString & cmd, const QStringList & args);

    bool flagStarted;
    bool flagFinished;
    bool flagBusy;
    QTextEdit * output;

signals:

    void sigOutput(const QString &);
    void sigErrorOutput(const QString &);
    void sigFinished(int);

public slots:

    void logOutput();
    void logErrorOutput();
    void logFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

//******************************************************************************
#endif // YPROCESS_H
