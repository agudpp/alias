#include <qt_client/handlers/content_editor_widget.h>

#include <QPushButton>
#include <QMessageBox>

#include <toolbox/debug/debug.h>
#include <qt_client/content/content_widget_builder.h>
#include <qt_client/common/converter_utils.h>
#include <qt_client/common/session_data.h>

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
  content_widget_->applyChanges();

  data::Content::Ptr content = content_widget_->ref();
  data::Metadata metadata = content->metadata();
  metadata.setEncrypted(ui->encrypt_checkBox->isChecked());
  content->setMetadata(metadata);

  const ContentData content_data{content, tagger_widget_->selectedTags()};
  emit storeContent(content_data);
}

void
ContentEditorWidget::onDeleteClicked(void)
{
  QMessageBox msgBox;
  msgBox.setText("Are you sure you want to delete this element.");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  const int ret = msgBox.exec();

  if (ret == QMessageBox::No) {
    return;
  }

  data::Content::Ptr content = content_widget_->ref();
  if (!service_api_->deleteContent(content->id())) {
    LOG_ERROR("Something happened deleting the content...")
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

void
ContentEditorWidget::configureEncryptionCheckbox(const data::Content::ConstPtr& content, bool editable)
{
  ui->encrypt_checkBox->setEnabled(editable);
  const bool encrypted = content->metadata().encrypted();
  ui->encrypt_checkBox->setCheckState(encrypted ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
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
  QObject::connect(ui->delete_button, &QPushButton::clicked,
                   this, &ContentEditorWidget::onDeleteClicked);

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
  ui->delete_button->hide();
  ui->content_type_combo_box->setEnabled(false);
  configureEncryptionCheckbox(content, false);
  configureNewContentWidget(ContentWidgetBuilder::buildReadOnly(content));
}

void
ContentEditorWidget::setEditableContent(data::Content::Ptr content)
{
  ui->save_button->show();
  ui->cancel_button->show();
  ui->delete_button->show();
  // we only can delete contents that are on the "backend"
  ui->delete_button->setEnabled(service_api_->hasContentWithId(content->id()));
  ui->content_type_combo_box->setEnabled(true);
  configureEncryptionCheckbox(content, true);
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
