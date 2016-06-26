/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 26.6.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef ICOMPLETER_H
#define ICOMPLETER_H

///
/// \brief The ICompleter class will be in charge of provide functionality to
///        get content from a given query and config values.
///        We will have different completers for different data we want to get
///        from the user, for example:
///        TagCompleter: will be used to get IContent elements from tags.
///        TextCompleter: will be used to get IContent from the text content given
///                       by the IContent elements.
///
///
class ICompleter
{
public:
    ICompleter();
    virtual ~ICompleter();


};

#endif // ICOMPLETER_H
