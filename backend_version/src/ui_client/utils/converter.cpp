#include "converter.h"

#include <core/debug/Debug.h>



TagWidget*
Converter::toWidget(const Tag::ConstPtr& tag)
{
  TagWidget* result = TagWidget::getOne();
  result->cleanObject();
  result->configure(tag);
  ASSERT_PTR(tag.get());
  return result;
}


