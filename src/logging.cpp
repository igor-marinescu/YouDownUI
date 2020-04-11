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
// Logging to file
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include "logging.h"
#include <QDateTime>

//******************************************************************************
// Constructor
//******************************************************************************
Logging::Logging() : ptrFile(nullptr), ptrStream(nullptr)
{
    ptrFile = new QFile("log.txt");
    if(ptrFile->open(QIODevice::Append | QIODevice::Text))
    {
        ptrStream = new QTextStream(ptrFile);
    }
    else
    {
        delete ptrFile;
        ptrFile = nullptr;
    }
}

//******************************************************************************
// Destructor
//******************************************************************************
Logging::~Logging()
{
    if(ptrStream != nullptr)
    {
        delete ptrStream;
    }
    if(ptrFile != nullptr)
    {
        delete ptrFile;
    }
}

//******************************************************************************
// Log string
//******************************************************************************
void Logging::out(const QString & txt)
{
    if(ptrStream != nullptr)
    {
        QString strDateTime = QDateTime::currentDateTime().toString( Qt::ISODate);
        *ptrStream << strDateTime << " " << txt << "\n";
    }
}
