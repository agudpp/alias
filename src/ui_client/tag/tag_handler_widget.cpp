#include "tag_handler_widget.h"

#include <core/utils/string_utils.h>
#include <core/debug/Debug.h>

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

#include <ui_client/tag/input_text_validator.h>

#include "ui_tag_handler_widget.h"


void
TagHandlerWidget::removeCurrentSelTag(void)
{
  if (!selected_tags_->hasSelection()) {
    return;
  }
  TagWidget* current = selected_tags_->selected();
  selected_tags_->popTag(current);
  emit tagRemoved(current->tag());
  freeWidget(current);
}

void
TagHandlerWidget::selectNextTag(TagListHandler* tag_handler, bool left_dir, bool only_if_selected, bool emit_signal)
{
  if (!tag_handler->hasSelection() && only_if_selected) {
    return;
  }

  if (left_dir) {
    emit_signal = tag_handler->selectPrev() && emit_signal;
  } else {
    emit_signal = tag_handler->selectNext() && emit_signal;
  }
  if (emit_signal && tag_handler->hasSelection()) {
    emit tagSelected(tag_handler->selected()->tag());
  }
}

TagWidget*
TagHandlerWidget::getWidget(Tag::ConstPtr& tag)
{
  TagWidget* result = nullptr;
  if (widgets_queue_.empty()) {
    result = new TagWidget;
  } else {
    result = widgets_queue_.back();
    widgets_queue_.pop_back();
  }
  result->configure(tag);
  return result;
}

void
TagHandlerWidget::freeWidget(TagWidget* widget)
{
  ASSERT_PTR(widget);
  widget->setParent(nullptr);
  widget->clear();
  widgets_queue_.push_back(widget);
}

TagWidget*
TagHandlerWidget::getOrCreateTag(const std::string& text, bool create_if_not_exists)
{
  Tag::ConstPtr tag;
  if (create_if_not_exists) {
    tag = service_api_->addTag(text);
    ASSERT_PTR(tag.get());
  } else {
    tag = service_api_->getTagByText(text);
  }

  return tag.get() != nullptr ? getWidget(tag) : nullptr;
}

void
TagHandlerWidget::popAndFreeWidgetsFromHandler(TagListHandler* handler)
{
  std::vector<TagWidget*> tags;
  handler->popAllTags(tags);
  for (TagWidget* w : tags) {
    freeWidget(w);
  }
}

std::vector<TagWidget*>
TagHandlerWidget::toTagWidgets(const std::set<Tag::ConstPtr>& tags)
{
  std::vector<TagWidget*> result;
  for (Tag::ConstPtr t : tags) {
    result.push_back(getWidget(t));
  }
  return result;
}

bool
TagHandlerWidget::lineEditEventFilter(QEvent *event)
{
  const bool is_key_release = event->type() == QEvent::KeyRelease;
  const bool is_key_press = event->type() == QEvent::KeyPress;

  if(!is_key_press && !is_key_release) {
    return false;
  }

  QKeyEvent *ke = static_cast<QKeyEvent *>(event);
  // We avoid propagaiting the tab key if pressed in here.
  if (is_key_press) {
    if (ke->key() == Qt::Key_Tab) {
      event->accept();
      return true;
    }
    return false;
  }

  const bool shift_pressed = ke->modifiers() & Qt::ShiftModifier;

  if (ke->key() == Qt::Key_Backspace) {
    // Check if we need to select / remove tag when empty text
    if (ui->lineEdit->text().isEmpty()) {
      if (selected_tags_->hasSelection()) {
        removeCurrentSelTag();
      } else if (selected_tags_->hasTags()){
        selected_tags_->select(selected_tags_->last());
        emit tagSelected(selected_tags_->selected()->tag());
      }
    }
  } else if (ke->key() == Qt::Key_Tab) {
    if (selected_tags_->hasSelection() || suggested_tags_->hasSelection()) {
      selectNextTag(selected_tags_, shift_pressed, true);
      selectNextTag(suggested_tags_, shift_pressed, true, false);
    } else {
      // autocomplete?
      const std::string current_text = ui->lineEdit->text().toStdString();
      const std::string expanded = core::StringUtils::shortestCommonSuffix(suggestedTagsTexts(), current_text);
      qDebug() << "current_text: " << current_text.c_str() << " expanded: " << expanded.c_str();
      if (expanded == current_text) {
        // we need to jump to the suggested tags to select one
        selectNextTag(suggested_tags_, shift_pressed, false, false);
      } else {
        ui->lineEdit->setText(expanded.c_str());
      }
    }
    event->accept();
    qDebug() << "accepting event tab";
    ui->lineEdit->setFocus();
    return true;
  } else if (ke->key() == Qt::Key_Escape) {
    selected_tags_->unselect(selected_tags_->selected());
    suggested_tags_->unselect(suggested_tags_->selected());
  } else if (ke->key() == Qt::Key_Return) {
    if (suggested_tags_->hasSelection()) {
      TagWidget* sel_tag = suggested_tags_->selected();
      suggested_tags_->popTag(sel_tag);
      selected_tags_->addTag(sel_tag);
      ui->lineEdit->clear();
      popAndFreeWidgetsFromHandler(suggested_tags_);
      emit tagSelected(sel_tag->tag());
      event->accept();
      return true;
    }
  } else if (ke->key() == Qt::Key_Space) {
    if (!ui->lineEdit->text().isEmpty()) {
      const std::string& tag_text = ServiceAPI::normalizeTagText(ui->lineEdit->text().toStdString());
      if (!selected_tags_->hasTagWithText(tag_text)) {
        TagWidget* new_tag = getOrCreateTag(tag_text, can_add_flag_);
        if (new_tag != nullptr) {
          selected_tags_->addTag(new_tag);
          emit tagSelected(new_tag->tag());
          ui->lineEdit->clear();
        }
      }
    }
    event->accept();
    return true;
  }

  emit someKeyPressed(ke);
  return false;
}

void
TagHandlerWidget::lineEditTextChanged(const QString& text)
{
  selected_tags_->unselect(selected_tags_->selected());
  emit inputTextChanged(text);
}



TagHandlerWidget::TagHandlerWidget(QWidget *parent, ServiceAPI* service_api) :
  QWidget(parent),
  ui(new Ui::TagHandlerWidget),
  service_api_(service_api),
  can_add_flag_(false)
{
  ASSERT_PTR(service_api);
  ui->setupUi(this);
  selected_tags_ = new TagListHandler;
  suggested_tags_ = new TagListHandler;
  selected_tags_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  suggested_tags_->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  ui->verticalLayout->addWidget(selected_tags_);
  ui->verticalLayout->addWidget(suggested_tags_);

  installEventFilter(this);
  InputTextValidator* validator = new InputTextValidator(this);
  ui->lineEdit->setValidator(validator);
  ui->lineEdit->installEventFilter(this);

  QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, &TagHandlerWidget::lineEditTextChanged);
}

TagHandlerWidget::~TagHandlerWidget()
{
  clear();
  while (!widgets_queue_.empty()) {
    delete widgets_queue_.back();
    widgets_queue_.pop_back();
  }
  delete ui;
}

void
TagHandlerWidget::setAddTagsFlag(bool can_add_flag)
{
  can_add_flag_ = can_add_flag;
}

void
TagHandlerWidget::setSuggestedTags(const std::set<Tag::ConstPtr>& tags)
{
  popAndFreeWidgetsFromHandler(suggested_tags_);
  suggested_tags_->setTags(toTagWidgets(tags));
}

void
TagHandlerWidget::setSelectedTags(const std::set<Tag::ConstPtr>& tags)
{
  popAndFreeWidgetsFromHandler(selected_tags_);
  selected_tags_->setTags(toTagWidgets(tags));
}


bool
TagHandlerWidget::eventFilter(QObject *object, QEvent *event)
{
  if (object == ui->lineEdit) {
    return lineEditEventFilter(event);
  }

  return false;
}

std::vector<std::string>
TagHandlerWidget::suggestedTagsTexts(void) const
{
  std::vector<std::string> tags_text;
  const std::vector<TagWidget*>& tags = suggested_tags_->tags();
  for (const TagWidget* tag : tags) {
    ASSERT_PTR(tag);
    ASSERT_PTR(tag->tag().get());
    tags_text.push_back(tag->tag()->text());
  }
  return tags_text;
}

std::vector<std::string>
TagHandlerWidget::selectedTagsTexts(void) const
{
  std::vector<std::string> tags_text;
  const std::vector<TagWidget*>& tags = selected_tags_->tags();
  for (const TagWidget* tag : tags) {
    tags_text.push_back(tag->tag()->text());
  }
  return tags_text;
}

bool
TagHandlerWidget::hasSelectedTags(void) const
{
  return selected_tags_->hasTags();
}

QString
TagHandlerWidget::currentText(void) const
{
  return ui->lineEdit->text();
}

void
TagHandlerWidget::clear(void)
{
  popAndFreeWidgetsFromHandler(selected_tags_);
  popAndFreeWidgetsFromHandler(suggested_tags_);
  ui->lineEdit->clear();
}

void
TagHandlerWidget::activate(void)
{
  setFocus();
  ui->lineEdit->setFocus();
}

