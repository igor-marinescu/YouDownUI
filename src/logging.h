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

#ifndef LOGGING_H
#define LOGGING_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QFile>
#include <QTextStream>

//******************************************************************************
// Includes
//******************************************************************************
#ifdef LOGGING
class Logging {

    QFile * ptrFile;
    QTextStream * ptrStream;

    Logging();
    Logging(Logging const &);           // don't implement
    
    ~Logging();

    void operator=(Logging const &);    // don't implement
    
public:

    void out(const QString & cmd);
    
    static Logging & getInstance()
    {
        static Logging instance;
        return instance;
    }    
};
#endif

#ifdef LOGGING
    #define LOG_OUT(qstr)   Logging::getInstance().out(qstr)
#else
    #define LOG_OUT(qstr)
#endif

//******************************************************************************
#endif // LOGGING_H
