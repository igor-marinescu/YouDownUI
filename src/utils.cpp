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
#include "utils.h"

#include <QDir>

/*******************************************************************************
 * @brief Return the location where the configuration is stored for Windows.
 *        Convert configuration filename to complete path (including filename):
 *
 *              "%APPDATA%\APP_NAME\<filename>"
 *
 * Note: APP_NAME must be defined (Ex: #define APP_NAME "MyApp")
 *
 * @param [in] string containing configuration filename (without path).
 * @return complete path (including filename) to location of configuration file.
 ******************************************************************************/
#if (defined (_WIN32) || defined (_WIN64))
QString get_config_location_win(QString filename)
{
    QString config_dir_name;
    char * ptr_str;
    size_t len;
    errno_t err = _dupenv_s(&ptr_str, &len, "appdata");
    if(!err)
    {
        config_dir_name = QString(ptr_str) + QDir::separator() + QString(APP_MAME);
        free(ptr_str);
        
        if(!QDir(config_dir_name).exists())
        {
            if(!QDir().mkdir(config_dir_name))
            {
                // Failed to create directory, store the settings along with the binary file.
                return filename;
            }
        }
        
        // Directory exists (or successfully created), return file name
        return (config_dir_name + QDir::separator() + filename);
    }

    // %APPDATA% not defined, the settings are stored along with the binary file.
    return filename;
}
#endif

/*******************************************************************************
 * @brief Return the location where the configuration is stored for Linux OS.
 *        Convert configuration filename to complete path (including filename):
 *
 * "$HOME/.config/APP_MAME/<filename>" or "$XDG_CONFIG_HOME/APP_NAME/<filename>"
 *
 * Note: APP_NAME must be defined (Ex: #define APP_NAME "MyApp")
 *
 * `$XDG_CONFIG_HOME` defines the base directory relative to which user-specific 
 * configuration files should be stored. If `$XDG_CONFIG_HOME` is either not set 
 * or empty, a default equal to `$HOME/.config` should be used.
 *
 * @param [in] string containing configuration filename (without path).
 * @return complete path (including filename) to location of configuration file.
 ******************************************************************************/
#if (defined (LINUX) || defined (__linux__))
QString get_config_location_linux(QString filename)
{
    QString config_dir_name;
    char * ptr_str = getenv("XDG_CONFIG_HOME");
    if(ptr_str)
    {
        config_dir_name = QString(ptr_str);
    }
    else{
        ptr_str = getenv("HOME");
        if(ptr_str)
        {
            config_dir_name = QString(ptr_str) + QDir::separator() + QString(".config");
        }
    }

    // If $XDG_CONFIG_HOME or $HOME defined, check if settings directory 
    // APP_MAME already exists, if not create it.
    if(ptr_str)
    {
        config_dir_name += QDir::separator() + QString(APP_MAME);
        if(!QDir(config_dir_name).exists())
        {
            if(!QDir().mkdir(config_dir_name))
            {
                // Failed to create directory, store the settings along with the binary file.
                return filename;
            }
        }
         
        // Directory exists (or successfully created), return file name
        return (config_dir_name + QDir::separator() + filename);
    }

    // If either $XDG_CONFIG_HOME or $HOME defined, the settings are stored 
    // along with the binary file.
    return filename;
}
#endif

/*******************************************************************************
 * @brief Return the location where the configuration is stored.
 *        Convert configuration filename to complete path (including filename)
 *        depending on operating system.
 * @param [in] string containing configuration filename (without path).
 * @return complete path (including filename) to location of configuration file.
 ******************************************************************************/
QString get_config_location(QString filename)
{    
#if (defined (_WIN32) || defined (_WIN64))
    return get_config_location_win(filename);
#elif (defined (LINUX) || defined (__linux__))
    return get_config_location_linux(filename);
#else
    return filename;
#endif
}
