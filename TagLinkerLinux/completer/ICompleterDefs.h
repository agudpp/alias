/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 26.6.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef ICOMPLETERDEFS_H
#define ICOMPLETERDEFS_H

#include <string>


///
/// \brief The Filters struct defining what kind of filter we want to apply
///
struct CompleterFilters {

};

///
/// \brief The Query struct what it will be the content of a query
///
struct CompleterQuery {
    // maybe this could be parsed into different keywords?
    std::string query;
    // the filters
    CompleterFilters filters;
};

#endif // ICOMPLETERDEFS_H

