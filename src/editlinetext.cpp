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
// Add/Edit line of text
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include "editlinetext.h"
#include "ui_editlinetext.h"

//******************************************************************************
EditLineText::EditLineText(QWidget *parent, QString lblInfo, QString * text) :
    QDialog(parent),
    ui(new Ui::EditLineText)
{
    ui->setupUi(this);

    ui->lblInfo->setText(lblInfo);
    ui->edtText->setText(*text);

    ptrText = text;
}

//******************************************************************************
EditLineText::~EditLineText()
{
    delete ui;
}

//******************************************************************************
void EditLineText::on_buttonBox_accepted()
{
    *ptrText = ui->edtText->text();
}
