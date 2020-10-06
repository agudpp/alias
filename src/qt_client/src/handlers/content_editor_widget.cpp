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
ContentEditorWidget::onContentComboBoxChanged(const QString& text)
{
  if (content_widget_ == nullptr) {
    // skip here since we don't have a content to edit / modify
    return;
  }

  // we change the type of the current editable ref() and we set it again here to
  // configure the editor and everything else again
  const data::ContentType content_type = data::fromString(text.toStdString());

  ASSERT(content_widget_->canEdit());
  // avoid losing the changes on the screen, note that this can show metadata information
  // added by the content type manipulator itself.
  content_widget_->applyChanges();

  auto content_ptr = content_widget_->ref();
  data::Metadata metadata = content_ptr->metadata();
  if (metadata.type() == content_type) {
    LOG_WARNING("We are triggering an event in the wrong way? Seems that the type changed "
                "but is the same.. so nothing we can do here");
    return;
  }


  metadata.setType(content_type);
  content_ptr->setMetadata(metadata);
  setEditableContent(content_ptr);
}

void
ContentEditorWidget::cleanCurrentContentWidgets()
{
  if (content_widget_ != nullptr) {
    delete content_widget_;
    content_widget_ = nullptr;
  }
}

void
ContentEditorWidget::setupContentWidget(ContentWidgetInterface* content_widget)
{
  ui->content_layout->insertWidget(1, content_widget);
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

void
ContentEditorWidget::configureContentComboBox(const data::Content::ConstPtr& content)
{
  const QString content_str = QString::fromStdString(data::toString(content->metadata().type()));
  ui->content_type_combo_box->setCurrentText(content_str);
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

  cleanCurrentContentWidgets();
  configureContentComboBox(content_wiget->anyRef());
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
  ui->verticalLayout->insertWidget(1, tagger_widget_);

  QObject::connect(ui->save_button, &QPushButton::clicked,
                   this, &ContentEditorWidget::onSaveClicked);
  QObject::connect(ui->cancel_button, &QPushButton::clicked,
                   this, &ContentEditorWidget::onCancelClicked);

  // configure combobox
  const std::vector<std::string> content_types = data::contentTypeStrings();
  ui->content_type_combo_box->clear();
  ui->content_type_combo_box->setEditable(false);
  for (const std::string& content_str : content_types) {
    ui->content_type_combo_box->addItem(QString::fromStdString(content_str));
  }
  QObject::connect(ui->content_type_combo_box, &QComboBox::currentTextChanged,
                   this, &ContentEditorWidget::onContentComboBoxChanged);
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
  ui->content_type_combo_box->setEnabled(false);
  configureNewContentWidget(ContentWidgetBuilder::buildReadOnly(content));
}

void
ContentEditorWidget::setEditableContent(data::Content::Ptr content)
{
  ui->save_button->show();
  ui->cancel_button->show();
  ui->content_type_combo_box->setEnabled(true);
  configureNewContentWidget(ContentWidgetBuilder::buildEditable(content));
}

void
ContentEditorWidget::clearAll()
{
  cleanCurrentContentWidgets();
  tagger_widget_->clearAll();
}

void
ContentEditorWidget::activate()
{
  if (content_widget_ != nullptr) {
    content_widget_->activate();
  }
}

} // namespace qt_client
