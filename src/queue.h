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
// Queue (Table View) of elements to be downloaded
//******************************************************************************
#ifndef QUEUE_H
#define QUEUE_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QTableWidget>

//******************************************************************************
// Includes
//******************************************************************************
class Queue {

    QTableWidget * tab;

public:

    // Queue elemenet
    typedef struct {
        QString link;
        QString format;
        QString extra;
        bool audio;
    } Element;

    Queue();
    ~Queue();

    void assignTab(QTableWidget * ptrTab);
    void resizeTab(int width);
    void releaseTab();

    bool add(const Element & element);
    bool get(int idx, Element * ptrElement);
    bool edit(int idx, const Element & element);
    bool del(int idx);
    void clear();

    bool load(QString filename);
    bool save(QString filename);

    static bool elementsEqual(const Element & el1, const Element & el2);
    static QString strElement(const Element * ptrElement);
};

//******************************************************************************
#endif // DFU_H
