#include <qt_client/handlers/content_editor_widget.h>

#include <QPushButton>

#include <toolbox/debug/debug.h>

#include <qt_client/content/content_widget_builder.h>
#include <qt_client/common/converter_utils.h>

#include "ui_content_editor_widget.h"


namespace qt_client {


void
ContentEditorWidget::onCancelClicked(void)
{
  // TODO: check what we want to do here on cancel
  close();
}

void
ContentEditorWidget::onSaveClicked(void)
{
  // we need to check if the content exists on the backend or we should create one
  content_widget_->applyChanges();

  data::Content::Ptr content = content_widget_->ref();
  ASSERT_PTR(content.get());

  // get the tags or create them from the backend
  const std::vector<data::Tag::ConstPtr> be_tags = getOrStoreTags(tagger_widget_->selectedTags());

  if (be_tags.empty()) {
    // TODO: show a QMEssageBox with the error
    LOG_WARNING("The content has no tags, hence cannot be identified, we will not create this");
    return;
  }

  // set the list of tags
  content->setTagIDs(ConverterUtils::toIdsSet(be_tags));

  data::Content::ConstPtr original_content;
  if (service_api_->getContentById(content->id(), original_content)) {
    ASSERT_PTR(original_content.get());
    // we need to update
    if (!service_api_->updateContent(original_content->id(), content)) {
      // TODO: show error message here
      LOG_ERROR("Error updating the content " << content->id());
    }
  } else {
    // we need to store a new one
    if (!service_api_->createContent(content->metadata().type(),
                                     false,
                                     content->data(),
                                     content->tagIDs())) {
      // TODO: show error message here
      LOG_ERROR("Error creating the content");
    }
  }

  close();
}

void
ContentEditorWidget::cleanCurrentContentWidget()
{
  if (content_widget_ != nullptr) {
    delete content_widget_;
    content_widget_ = nullptr;
  }
}

void
ContentEditorWidget::setupContentWidget(ContentWidgetInterface* content_widget)
{
  ui->verticalLayout->insertWidget(0, content_widget);
  content_widget_ = content_widget;
}

std::vector<data::Tag::ConstPtr>
ContentEditorWidget::contentTags(const data::Content::ConstPtr& content)
{
  std::vector<data::Tag::ConstPtr> result;
  std::vector<toolbox::UID> ids;
  for (auto& tag_id : content->tagIDs()) {
    ids.push_back(tag_id);
  }

  if (!service_api_->getTagsByIds(ids, result)) {
    LOG_ERROR("Something happened when trying to get the tags from the ids");
  }

  return result;
}

std::vector<data::Tag::ConstPtr>
ContentEditorWidget::getOrStoreTags(const std::vector<data::Tag::ConstPtr>& tags)
{
  std::vector<data::Tag::ConstPtr> result;
  for (auto& tag : tags) {
    data::Tag::ConstPtr be_tag;
    if (service_api_->getTagByName(tag->name(), be_tag)) {
      result.push_back(be_tag);
    } else {
      result.push_back(service_api_->createTag(tag->name()));
    }
  }
  return result;
}

void
ContentEditorWidget::setupTagger(const std::vector<data::Tag::ConstPtr>& tags)
{
  tagger_widget_->setSelectedTags(tags);
}

void
ContentEditorWidget::configureNewContentWidget(ContentWidgetInterface* content_wiget)
{
  ASSERT_PTR(content_wiget);

  cleanCurrentContentWidget();
  setupContentWidget(content_wiget);
  setupTagger(contentTags(content_wiget->anyRef()));

}

ContentEditorWidget::ContentEditorWidget(QWidget* parent,
                                         service::ServiceAPI::Ptr service_api) :
  QWidget(parent)
, ui(new Ui::ContentEditorWidget)
, content_widget_(nullptr)
, service_api_(service_api)
{
  ui->setupUi(this);

  tagger_widget_ = new TaggerWidget(this, service_api);
  ui->verticalLayout->insertWidget(0, tagger_widget_);

  QObject::connect(ui->save_button, &QPushButton::clicked,
                   this, &ContentEditorWidget::onSaveClicked);
  QObject::connect(ui->cancel_button, &QPushButton::clicked,
                   this, &ContentEditorWidget::onCancelClicked);
}

ContentEditorWidget::~ContentEditorWidget()
{
  delete ui;
}


void
ContentEditorWidget::setReadOnlyContent(data::Content::ConstPtr content)
{
  ui->save_button->hide();
  ui->cancel_button->hide();
  configureNewContentWidget(ContentWidgetBuilder::buildReadOnly(content));
}

void
ContentEditorWidget::setEditableContent(data::Content::Ptr content)
{
  ui->save_button->show();
  ui->cancel_button->show();
  configureNewContentWidget(ContentWidgetBuilder::buildEditable(content));
}

void
ContentEditorWidget::clearAll()
{
  cleanCurrentContentWidget();
  tagger_widget_->clearAll();
}

} // namespace qt_client
