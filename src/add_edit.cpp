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
    in_constructor = true;

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
    ui->radioBtnVideo->setChecked(!ptrElement->audio);
    ui->radioBtnAudio->setChecked(ptrElement->audio);
    ui->edtLink->setText(ptrElement->link);
    ui->edtExtra->setText(ptrElement->extra);

    // In case of a Video, select the format index
    if(!ptrElement->audio){
        ui->cmbboxFormat->setCurrentIndex(idxFormat);
    }
    else{
        // In case of an Audio, select format "<default>" and disable the list
        ui->cmbboxFormat->setCurrentIndex(0);
        ui->cmbboxFormat->setEnabled(false);
    }

    in_constructor = false;
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

    ptrElement->audio = ui->radioBtnAudio->isChecked();
    ptrElement->link = strLink;
    ptrElement->format = strFormat;
    ptrElement->extra = strExtra;
}

//******************************************************************************
// Audio/Video Radiobuttons toggled
//******************************************************************************
void AddEdit::on_radioBtnVideo_toggled(bool checked)
{
    if(in_constructor)
        return;

    if(checked){
        ui->cmbboxFormat->setEnabled(true);
    }
}

void AddEdit::on_radioBtnAudio_toggled(bool checked)
{
    if(in_constructor)
        return;

    if(checked){
        ui->cmbboxFormat->setCurrentIndex(0);
        ui->cmbboxFormat->setEnabled(false);
    }
}
