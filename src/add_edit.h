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
#ifndef ADD_EDIT_H
#define ADD_EDIT_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QDialog>
#include "settings.h"
#include "queue.h"

//******************************************************************************
namespace Ui {
class AddEdit;
}

//******************************************************************************
class AddEdit : public QDialog
{
    Q_OBJECT
    bool in_constructor;

public:
    explicit AddEdit(QWidget *parent, Settings::SettingsData * settings,
                     Queue::Element * element);
    ~AddEdit();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddEdit *ui;
    Queue::Element * ptrElement;
};

//******************************************************************************
#endif // ADD_EDIT_H
