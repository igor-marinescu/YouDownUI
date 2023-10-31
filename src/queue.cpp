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

//******************************************************************************
// Includes
//******************************************************************************
#include "queue.h"
#include <QFile>
#include <QTextStream>

#define QSTR_AUDIO  QString("Audio")
#define QSTR_VIDEO  QString("Video")

//******************************************************************************
// Constructor
//******************************************************************************
Queue::Queue() : tab(nullptr)
{

}

//******************************************************************************
// Destructor
//******************************************************************************
Queue::~Queue()
{

}

//******************************************************************************
// Assign Table Widget to queue class
//******************************************************************************
void Queue::assignTab(QTableWidget * ptrTab)
{
    tab = ptrTab;
}

//******************************************************************************
// Release Table Widget from queue class
//******************************************************************************
void Queue::releaseTab()
{
    tab = nullptr;
}

//******************************************************************************
// Resize Table Widget columns
//******************************************************************************
void Queue::resizeTab(int width)
{
    if(tab == nullptr)
        return;

    tab->setColumnWidth(0, width * 0.05);  // Video/Adio
    tab->setColumnWidth(1, width * 0.50);  // Link
    tab->setColumnWidth(2, width * 0.35);  // Format
    tab->setColumnWidth(3, width * 0.05);  // Extra
}

//******************************************************************************
// Add element to queue
// param pointer to Element to add to list
// return true - element successfuly added, false - error adding element
//******************************************************************************
bool Queue::add(const Element & element){

    QTableWidgetItem * ptrItem;

    if(element.link.isEmpty() || (tab == nullptr))
        return false;

    int rowCount = tab->rowCount();

    // add element to table
    tab->setRowCount(rowCount + 1);
    // video/audio
    ptrItem = new QTableWidgetItem(element.audio ? QSTR_AUDIO : QSTR_VIDEO);
    ptrItem->setFlags(ptrItem->flags() & ~Qt::ItemIsEditable);
    tab->setItem(rowCount, 0, ptrItem);
    // link
    ptrItem = new QTableWidgetItem(element.link);
    ptrItem->setFlags(ptrItem->flags() & ~Qt::ItemIsEditable);
    tab->setItem(rowCount, 1, ptrItem);
    // format
    ptrItem = new QTableWidgetItem(element.format);
    ptrItem->setFlags(ptrItem->flags() & ~Qt::ItemIsEditable);
    tab->setItem(rowCount, 2, ptrItem);
    // extra
    ptrItem = new QTableWidgetItem(element.extra);
    ptrItem->setFlags(ptrItem->flags() & ~Qt::ItemIsEditable);
    tab->setItem(rowCount, 3, ptrItem);

    return true;
}

//******************************************************************************
// Get element from queue (but don't delete it)
// param    idx - element index in list
// param    ptrElement - pointer to element where data will be copied
// return   true - element data copied in ptrElement, false - element index out of range
//******************************************************************************
bool Queue::get(int idx, Element * ptrElement)
{
    QTableWidgetItem * ptrItem;
    QString strAudio, strLink, strFormat, strExtra;

    if(tab == nullptr)
        return false;

    int rowCount = tab->rowCount();

    if((idx >= 0) && (idx < rowCount))
    {
        // get audio/video
        ptrItem = tab->item(idx, 0);
        if(ptrItem != nullptr)
            strAudio = ptrItem->text();

        // get link
        ptrItem = tab->item(idx, 1);
        if(ptrItem != nullptr)
            strLink = ptrItem->text();

        // get format
        ptrItem = tab->item(idx, 2);
        if(ptrItem != nullptr)
            strFormat = ptrItem->text();

        // get extra
        ptrItem = tab->item(idx, 3);
        if(ptrItem != nullptr)
            strExtra = ptrItem->text();

        // copy to element
        ptrElement->audio = (strAudio == QSTR_AUDIO);
        ptrElement->link = strLink;
        ptrElement->format = strFormat;
        ptrElement->extra = strExtra;
        return true;
    }

    return false;
}

//******************************************************************************
// Edit element from queue (change it's values)
// param    idx - element index in list
// param    ptrElement - pointer to element containing new data
// return   true - element data changes, false - element index out of range or data invalid
//******************************************************************************
bool Queue::edit(int idx, const Element & element)
{
    QTableWidgetItem * ptrItem;

    if(tab == nullptr)
        return false;

    if(element.link.isEmpty())
        return false;

    int rowCount = tab->rowCount();

    if((idx >= 0) && (idx < rowCount))
    {
        // get link
        ptrItem = tab->item(idx, 0);
        if(ptrItem != nullptr)
            ptrItem->setText(element.audio ? QSTR_AUDIO : QSTR_VIDEO);
        else
            return false;

        // get link
        ptrItem = tab->item(idx, 1);
        if(ptrItem != nullptr)
            ptrItem->setText(element.link);
        else
            return false;

        // get format
        ptrItem = tab->item(idx, 2);
        if(ptrItem != nullptr)
            ptrItem->setText(element.format);
        else
            return false;

        // get extra
        ptrItem = tab->item(idx, 3);
        if(ptrItem != nullptr)
            ptrItem->setText(element.extra);
        else
            return false;

        return true;
    }

    return false;
}

//******************************************************************************
// Delete Element from Queue
// param idx - index of the element to be removed from list
// return true - element successfully removed, false - element index out of range
//******************************************************************************
bool Queue::del(int idx)
{
    QTableWidgetItem * ptrItem, * ptrItm2;

    if(tab == nullptr)
        return false;

    int rowCount = tab->rowCount();

    if((idx < 0) || (idx >= rowCount))
        return false;

    // move the content of the table one row up
    for(int i = idx; i < rowCount - 1; i++)
    {
        for(int j = 0; j < tab->columnCount(); j++)
        {
            ptrItem = tab->item(i, j);
            ptrItm2 = tab->item(i + 1, j);
            if((ptrItem != nullptr) && (ptrItm2 != nullptr))
            {
                ptrItem->setText(ptrItm2->text());
            }
        }
    }

    // remove last row
    tab->setRowCount(rowCount - 1);
    rowCount = tab->rowCount();
    return true;
}

//******************************************************************************
// Clear Queue
//******************************************************************************
void Queue::clear()
{
    if(tab != nullptr)
    {
        tab->setRowCount(0);
    }
}

//******************************************************************************
// Load file to Queue
//******************************************************************************
bool Queue::load(QString filename)
{
    QString strAudio;

    // clear old content
    clear();

    if(filename.isEmpty())
        filename = "queue.txt";

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    Element element;

    QTextStream in(&file);
    while(!in.atEnd()){
        strAudio = in.readLine();
        if(strAudio == QSTR_AUDIO){
            element.audio = true;
            element.link = in.readLine();
        }
        else if(strAudio == QSTR_VIDEO){
            element.audio = false;
            element.link = in.readLine();
        }
        else{
            element.audio = false;
            // Audio field is invalid, it doesn't contain eather "Audio" or "Video"
            // mots probablly this is an old file (where Audio was not present)
            // in this case treat Audio as Link
            element.link = strAudio;
        }
        element.format = in.readLine();
        element.extra = in.readLine();
        add(element);
    }
    return true;
}

//******************************************************************************
// Save Queue to file
//******************************************************************************
bool Queue::save(QString filename)
{
    QTableWidgetItem * ptrItem;

    if(filename.isEmpty())
        filename = "queue.txt";

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    for(int i = 0; i < tab->rowCount(); i++)
    {
        for(int j = 0; j < tab->columnCount(); j++)
        {
            ptrItem = tab->item(i, j);
            if(ptrItem != nullptr)
            {
                out << ptrItem->text() << "\n";
            }
        }
    }
    return true;
}

//******************************************************************************
// Return true if both elements are equal
//******************************************************************************
bool Queue::elementsEqual(const Element & el1, const Element & el2)
{
    return ((el1.audio == el2.audio) && (el1.link == el2.link) && (el1.format == el2.format) && (el1.link == el2.link));
}

//******************************************************************************
// Generate String from element
//******************************************************************************
QString Queue::strElement(const Element * ptrElement){
    if(ptrElement != nullptr)
    {
        return ((ptrElement->audio ? QSTR_AUDIO : QSTR_VIDEO) + " | "
                + ptrElement->link + " | " + ptrElement->format + " | " + ptrElement->extra);
    }
    return QString("nullptr");
}
