/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2017 by Happen
 *
 *   File  created by agustin on 1.7.2017
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <string>

class TagManager;
class ElementManager;

class DataStorage
{
public:
    struct MainData {
        TagManager* tagMngr;
        ElementManager* elemMngr;

        MainData(TagManager* aTagMngr = 0, ElementManager* aElemMngr = 0) :
            tagMngr(aTagMngr)
        ,   elemMngr(aElemMngr)
        {}
    };
public:
    DataStorage();
    ~DataStorage();

    ///
    /// \brief init
    /// \param md
    ///
    void
    init(const MainData& md);

    ///
    /// \brief saveToFile
    /// \param fname
    /// \return
    ///
    bool
    saveToFile(const std::string& fname);

    ///
    /// \brief loadFromFile
    /// \param fname
    /// \return
    ///
    bool
    loadFromFile(const std::string& fname);


private:
    ElementManager* m_elementMngr;
    TagManager* m_tagMngr;
};

#endif // DATASTORAGE_H
