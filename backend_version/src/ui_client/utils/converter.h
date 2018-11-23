#ifndef CONVERTER_H
#define CONVERTER_H

#include <tags/tag.h>

#include <ui_client/tag/tag_widget.h>



class Converter
{
  public:


    static TagWidget*
    toWidget(const Tag::Ptr& tag);

};

#endif // CONVERTER_H
