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
// Configuration File
//
// Igor Marinescu
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include "ConfFile.h"

#include <fstream>
#include <algorithm>
#include <sstream>

//******************************************************************************
// assigmnet operator
//******************************************************************************
ConfFile::ConfObj& ConfFile::ConfObj::operator=(const int& other){

    type = ConfFile::ConfObj::INTEGER;
    varInteger = other;
    varString.clear();

    return *this;
}

//******************************************************************************
// assigment operator
//******************************************************************************
ConfFile::ConfObj& ConfFile::ConfObj::operator=(const string& other){

    type = ConfFile::ConfObj::STRING;
    varString = other;
    varInteger = 0;

    return *this;
}

//******************************************************************************
// conversion operator
//******************************************************************************
ConfFile::ConfObj::operator int() {

    if(type == ConfObj::STRING){
        char * offset;
        varInteger = (int) strtol(varString.c_str(), &offset, 10);
        varString.clear();
    }
    type = ConfObj::INTEGER;
    return varInteger;
}

//******************************************************************************
// conversion operator
//******************************************************************************
ConfFile::ConfObj::operator string(){

    if(type == ConfObj::INTEGER){
        stringstream ss;
        ss << varInteger;
        varString = ss.str();
        varInteger = 0;
    }
    type = ConfObj::STRING;
    return varString;
}

//******************************************************************************
std::ostream& operator<<(std::ostream& os, const ConfFile::ConfObj & obj){
    if(obj.type == ConfFile::ConfObj::STRING)
        return os << obj.varString;
    return os << obj.varInteger;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
ConfFile::~ConfFile(){

    // Check if map must be dumped to a file
    if(filename.empty())
        return;

    // Create output file
    ofstream ofstr(filename.c_str());
    if(!ofstr.is_open())
        return;

    for(MapCatName::iterator cat = varMap.begin(); cat != varMap.end(); cat++){
        for(MapNameObj::iterator var = (*cat).second.begin(); var != (*cat).second.end(); var++){
            ofstr << (*cat).first << ":" << (*var).first << "=" << (*var).second << endl;
        }
    }
}

//******************************************************************************
ConfFile::ConfFile(const ConfFile& other){
    // copy map
    varMap = other.varMap;
}

//******************************************************************************
ConfFile& ConfFile::operator=(const ConfFile& rhs){

    if (this == &rhs)
        return *this; // handle self assignment

    varMap = rhs.varMap;

    return *this;
}

//******************************************************************************
ConfFile::ConfFile(const char * filename){

    string line, category, name, value;
    ifstream ifstr(filename);

    this->filename = filename;

    // Check if file is open
    if(!ifstr.is_open())
        return;

    size_t pos1, pos2;

    // Extract all variables/lines from file
    while(getline(ifstr, line)){

        //----------------------------------------------------------------------
        // Get category
        //----------------------------------------------------------------------
        pos1 = line.find(':');
        if(pos1 != string::npos){
            // Extract category and trim spaces
            category = line.substr(0, pos1);
            category.erase(remove_if(category.begin(), category.end(),
                    (int(*)(int))isspace), category.end());
        }
        else
            // No category in this line, ignore it
            continue;

        //----------------------------------------------------------------------
        // Get name
        //----------------------------------------------------------------------
        pos2 = line.find('=', pos1 + 1);
        if(pos2 != string::npos){
            // Extract name and trim spaces
            name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            name.erase(remove_if(name.begin(), name.end(),
                    (int(*)(int))isspace), name.end());
        }
        else
            // No name in this line, ignore it
            continue;

        //----------------------------------------------------------------------
        // Get value
        //----------------------------------------------------------------------
        value = line.substr(pos2 + 1);
        // remove spaces
        //value.erase(remove_if(value.begin(), value.end(),
        //            (int(*)(int))isspace), value.end());
        varMap[category][name] = value;

        //cout << "cat='" << category << "' name='" << name << "' value='" << value << "'" << endl;
    }
}

//******************************************************************************
bool ConfFile::exists(const string & cat){

    return (varMap.find(cat) != varMap.end());
}

//******************************************************************************
bool ConfFile::exists(const string & cat, const string & name){

    MapCatName::iterator it = varMap.find(cat);
    if(it != varMap.end())
        return ((*it).second.find(name) != (*it).second.end());

    return false;
}

//******************************************************************************
void ConfFile::set(const string & cat, const string & name, const int val){
    varMap[cat][name] = val;
}

//******************************************************************************
void ConfFile::set_str(const string & cat, const string & name, const string & val){
    varMap[cat][name] = val;
}

//******************************************************************************
void ConfFile::set_qstr(const char * cat, const char * name, const QString & qstr){
    varMap[cat][name] = qstr.toStdString();
}

//******************************************************************************
bool ConfFile::get(const string & cat, const string & name, int * val){

    MapCatName::iterator it = varMap.find(cat);
    if(it == varMap.end())
        return false;

    MapNameObj::iterator it2 = it->second.find(name);
    if(it2 == it->second.end())
        return false;

    if(val)
        *val = it2->second;

    return true;
}

//******************************************************************************
int ConfFile::get(const string & cat, const string & name, int default_value){

    MapCatName::iterator it = varMap.find(cat);
    if(it == varMap.end())
        return default_value;

    MapNameObj::iterator it2 = it->second.find(name);
    if(it2 == it->second.end())
        return default_value;

    return it2->second;
}

//******************************************************************************
bool ConfFile::get_str(const string & cat, const string & name, string * val){

    MapCatName::iterator it = varMap.find(cat);
    if(it == varMap.end())
        return false;

    MapNameObj::iterator it2 = it->second.find(name);
    if(it2 == it->second.end())
        return false;

    if(val){
        (*val).assign(it2->second);
    }

    return true;
}

//******************************************************************************
QString ConfFile::get_qstr(const char * cat, const char * name, const char * def){

    MapCatName::iterator it = varMap.find(cat);
    if(it == varMap.end()){
        if(def)
            return QString(def);
        else
            return QString();
    }

    MapNameObj::iterator it2 = it->second.find(name);
    if(it2 == it->second.end()){
        if(def)
            return QString(def);
        else
            return QString();
    }

    return QString(it2->second.varString.c_str());
}

//******************************************************************************
void ConfFile::clear(){
    varMap.clear();
}

//******************************************************************************
void ConfFile::clear(const string & cat){
    varMap.erase(cat);
}

//******************************************************************************
void ConfFile::clear(const string & cat, const string & name){

    MapCatName::iterator it = varMap.find(cat);
    if(it == varMap.end())
        return;

    (*it).second.erase(name);
}
