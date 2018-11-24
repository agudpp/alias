#ifndef CONVERTER_H
#define CONVERTER_H

#include <vector>
#include <set>

#include <tags/tag.h>

#include <ui_client/tag/tag_widget.h>



class Converter
{
  public:


    static TagWidget*
    toWidget(const Tag::ConstPtr& tag);


    template<typename T>
    static inline std::vector<TagWidget*>
    toWidget(const T& tags)
    {
      std::vector<TagWidget*> result;
      for (const Tag::ConstPtr& t : tags) {
        result.push_back(toWidget(t));
      }
      return result;
    }

};

#endif // CONVERTER_H
