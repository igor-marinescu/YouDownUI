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
// Util. functions and definitions
//******************************************************************************
#ifndef UTILS_H
#define UTILS_H

//******************************************************************************
// Includes
//******************************************************************************
#include <QString>

//******************************************************************************
// Defines
//******************************************************************************

//******************************************************************************
// Functions
//******************************************************************************

// Return the location where the configuration is stored
QString get_config_location(QString filename);

//******************************************************************************
#endif // UTILS_H
