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
#ifndef EDITLINETEXT_H
#define EDITLINETEXT_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QDialog>

//******************************************************************************
namespace Ui {
class EditLineText;
}

//******************************************************************************
class EditLineText : public QDialog
{
    Q_OBJECT

public:
    explicit EditLineText(QWidget *parent, QString lblInfo, QString * text);
    ~EditLineText();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditLineText *ui;
    QString * ptrText;
};

//******************************************************************************
#endif // EDITLINETEXT_H
