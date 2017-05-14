/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2017 by Happen
 *
 *   File  created by agustin on 14.5.2017
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <string>


class DataStorage
{
public:
    DataStorage();
    ~DataStorage();

    bool
    loadFromFile(const std::string& path);
};

#endif // DATASTORAGE_H
