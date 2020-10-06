#include <qt_client/content/content_widget_builder.h>

#include <toolbox/debug/debug.h>

#include <qt_client/content/text_content_widget.h>


namespace qt_client {

static ContentWidgetInterface*
buildFromType(data::ContentType content_type)
{
  ContentWidgetInterface* result = nullptr;

  switch (content_type) {
    case data::ContentType::TEXT:
      result = new TextContentWidget();
      break;
    case data::ContentType::COMMAND:
      // for now we will use the same ContentWidget that the Text one
      result = new TextContentWidget();
      break;
    default:
      LOG_ERROR("We cannot recognize the type of the content?: " << int32_t(content_type));
  }

  return result;
}


ContentWidgetInterface*
ContentWidgetBuilder::buildReadOnly(data::Content::ConstPtr ref)
{
  ContentWidgetInterface* result = buildFromType(ref->metadata().type());

  if (result != nullptr) {
    result->setConstRef(ref);
  }

  return result;
}

ContentWidgetInterface*
ContentWidgetBuilder::buildEditable(data::Content::Ptr ref)
{
  ContentWidgetInterface* result = buildFromType(ref->metadata().type());

  if (result != nullptr) {
    result->setRef(ref);
  }

  return result;

}


} // namespace qt_client
