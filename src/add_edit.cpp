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
// Add/Edit Link Element
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include "add_edit.h"
#include "ui_add_edit.h"

//******************************************************************************
// Constructor
//******************************************************************************
AddEdit::AddEdit(QWidget *parent, Settings::SettingsData * settings, Queue::Element * element) :
    QDialog(parent),
    ui(new Ui::AddEdit)
{
    ui->setupUi(this);
    ptrElement = element;
    int idxFormat = 0, idx = 0;

    // Build combobox list (meanwhile find format index of input element)
    ui->cmbboxFormat->addItem("<default>");
    QStringListIterator defFormatIt(settings->defFormats);
    while(defFormatIt.hasNext()){
        idx++;
        QString strFormat = defFormatIt.next();
        ui->cmbboxFormat->addItem(strFormat);
        if(!element->format.isEmpty() && (element->format == strFormat))
            idxFormat = idx;
    }

    // Display element
    ui->edtLink->setText(ptrElement->link);
    ui->cmbboxFormat->setCurrentIndex(idxFormat);
    ui->edtExtra->setText(ptrElement->extra);
}

//******************************************************************************
// Destructor
//******************************************************************************
AddEdit::~AddEdit()
{
    delete ui;
}

//******************************************************************************
// Copy element value
//******************************************************************************
void AddEdit::on_buttonBox_accepted()
{
    QString strLink, strFormat, strExtra;

    strLink = ui->edtLink->text();

    if(ui->cmbboxFormat->currentIndex() > 0)
        strFormat = ui->cmbboxFormat->currentText();

    strExtra = ui->edtExtra->text();

    ptrElement->link = strLink;
    ptrElement->format = strFormat;
    ptrElement->extra = strExtra;
}
