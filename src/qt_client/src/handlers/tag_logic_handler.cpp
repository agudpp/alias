#include <qt_client/handlers/tag_logic_handler.h>

#include <algorithm>

#include <QLineEdit>

#include <toolbox/utils/string_utils.h>
#include <toolbox/debug/debug.h>

#include <qt_client/handlers/input_text_validator.h>
#include <qt_client/common/converter_utils.h>


namespace qt_client {


///
/// \brief Will normalize a tag text
/// \param text the text to normalize
/// \return the normalized tag text
///

static std::string
normalizeTagText(const std::string& text)
{
  // TODO: here will be good if we normalize also special characters or
  // other things.
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  result.erase(std::remove_if(result.begin(), result.end(), [](char c){return std::isspace(c);}), result.end());

  return result;
}

void
TagLogicHandler::addSimpleKeyTrigger(Qt::Key key, QEvent::Type type, bool (TagLogicHandler::* fun)(QKeyEvent* key_event))
{
  KeyTrigger::Configuration config(key);
  config.event_type = type;
  FunctionKeyTrigger* key_trigger = new FunctionKeyTrigger(config, std::bind(fun, this, std::placeholders::_1));
  key_triggers_.push_back(KeyTrigger::Ptr(key_trigger));
}

void
TagLogicHandler::buildKeyTriggers(void)
{
  // TODO / Improvement: add a key map to function here for personalized configuration
  addSimpleKeyTrigger(Qt::Key_Tab, QEvent::KeyPress, &TagLogicHandler::onTabPressed);
  addSimpleKeyTrigger(Qt::Key_Backspace, QEvent::KeyRelease, &TagLogicHandler::onBackspacePressed);
  addSimpleKeyTrigger(Qt::Key_Escape, QEvent::KeyRelease, &TagLogicHandler::onEscapePressed);
  addSimpleKeyTrigger(Qt::Key_Return, QEvent::KeyRelease, &TagLogicHandler::onReturnPressed);
  addSimpleKeyTrigger(Qt::Key_Space, QEvent::KeyRelease, &TagLogicHandler::onSpacePressed);
}

bool
TagLogicHandler::onTabPressed(QKeyEvent* key_event)
{
  const bool shift_pressed = key_event->modifiers() & Qt::ShiftModifier;
  if (tag_suggestion_widget_->hasHighlighted()) {
    TagWidget* next_tag = shift_pressed ? tag_suggestion_widget_->prev()
                                        : tag_suggestion_widget_->next();
    tag_suggestion_widget_->highlight(next_tag);
    emit suggestedTagHighlightingChanged();
  } else {
    // autocomplete?
    const std::string current_text = line_edit_->text().toStdString();
    auto suggested_tag_texts = ConverterUtils::toTextVec(tag_suggestion_widget_->tags());
    const std::string expanded =
        toolbox::StringUtils::shortestCommonSuffix(suggested_tag_texts, current_text);
    LOG_INFO("current_text: " << current_text << " expanded: " << expanded);
    if (expanded == current_text) {
      // we need to jump to the suggested tags to select one
      TagWidget* next_tag = tag_suggestion_widget_->first();
      tag_suggestion_widget_->highlight(next_tag);
      emit suggestedTagHighlightingChanged();
    } else {
      line_edit_->setText(expanded.c_str());
    }
  }

  key_event->accept();
  line_edit_->setFocus();
  return true;
}

bool
TagLogicHandler::onBackspacePressed(QKeyEvent* key_event)
{
  // Check if we need to select / remove tag when empty text
  if (line_edit_->text().isEmpty()) {
    if (should_delete_tag_) {
      if (tag_list_widget_->hasTags()) {
        TagWidget* tag_removed = tag_list_widget_->popTag();
        emit tagRemoved(tag_removed);
      }
    } else {
      should_delete_tag_ = true;
    }
  }
  return false;
}

bool
TagLogicHandler::onEscapePressed(QKeyEvent* key_event)
{
  if (tag_suggestion_widget_->hasHighlighted()) {
    tag_suggestion_widget_->unhighlightAll();
    emit suggestedTagHighlightingChanged();
  }

  return false;
}

bool
TagLogicHandler::onReturnPressed(QKeyEvent* key_event)
{
  if (tag_suggestion_widget_->hasHighlighted()) {
    TagWidget* sel_tag = tag_suggestion_widget_->currentHighlighted();
    tag_suggestion_widget_->removeTag(sel_tag);
    tag_list_widget_->pushTag(sel_tag);
    line_edit_->clear();
    emit suggestedTagSelected(sel_tag);
    key_event->accept();
    return true;
  }
  return false;
}

bool
TagLogicHandler::onSpacePressed(QKeyEvent* key_event)
{
  if (!line_edit_->text().isEmpty()) {
    const std::string& tag_text = normalizeTagText(line_edit_->text().toStdString());
    emit tagCreationIntention(QString::fromStdString(tag_text));
  }
  key_event->accept();
  return true;
}

void
TagLogicHandler::lineEditTextChanged(const QString& text)
{
  should_delete_tag_ = false;
  if (tag_suggestion_widget_->hasHighlighted()) {
    tag_suggestion_widget_->unhighlightAll();
    // TODO: should emit signal here?
  }
  emit inputTextChanged(text);
}

void
TagLogicHandler::onTagButtonClosed()
{
  QObject* caller = sender();
  if (caller != nullptr) {
    TagWidget* tag_caller = static_cast<TagWidget*>(caller);
    LOG_INFO("Tag " << tag_caller->tag() << " closed?");
    emit tagRemoved(tag_caller);
  }
}

bool
TagLogicHandler::lineEditEventFilter(QEvent *event)
{
  if (event->type() != QEvent::KeyPress && event->type() != QEvent::KeyRelease) {
    return false;
  }
  QKeyEvent *ke = static_cast<QKeyEvent *>(event);
  for (KeyTrigger::Ptr& kt : key_triggers_) {
    if (kt->shouldTrigger(ke)) {
      if (kt->trigger(ke)) {
        return true;
      }
    }
  }

  if (ke->type() == QEvent::KeyRelease) {
    emit unhandledKeyEvent(ke);
  }
  return false;
}


TagLogicHandler::TagLogicHandler(TagListWidget* tag_list_widget,
                                 TagSuggestionListWidget* tag_suggestion_widget,
                                 QLineEdit* line_edit) :
  tag_list_widget_(tag_list_widget)
, tag_suggestion_widget_(tag_suggestion_widget)
, line_edit_(line_edit)
{
  // TODO: does the input text validator get freed when deleting the line edit?
  InputTextValidator* validator = new InputTextValidator(this);
  line_edit_->setValidator(validator);
  line_edit_->installEventFilter(this);

  QObject::connect(line_edit, &QLineEdit::textEdited, this, &TagLogicHandler::lineEditTextChanged);

  buildKeyTriggers();
}

TagLogicHandler::~TagLogicHandler()
{
  line_edit_->removeEventFilter(this);
  // TODO: remove validator?
}


bool
TagLogicHandler::eventFilter(QObject *object, QEvent *event)
{
  if (object == line_edit_) {
    return lineEditEventFilter(event);
  }

  return false;
}

void
TagLogicHandler::configure(const std::vector<TagWidget*>& current_tags,
                           const std::vector<TagWidget*>& suggested_tags,
                           bool clear_input)
{
  tag_list_widget_->popAllTags();
  tag_list_widget_->pushTags(current_tags);
  for (auto& current_tag : current_tags) {
    QObject::connect(current_tag, &TagWidget::onCloseClicked, this, &TagLogicHandler::onTagButtonClosed);
  }
  tag_suggestion_widget_->popAllTags();
  tag_suggestion_widget_->pushTags(suggested_tags);
  if (clear_input) {
    line_edit_->clear();
  }
}

void
TagLogicHandler::clearAll()
{
  line_edit_->clear();
  tag_list_widget_->popAllTags();
  tag_suggestion_widget_->popAllTags();
}

TagWidget*
TagLogicHandler::suggestedHighlightedTag() const
{
  return tag_suggestion_widget_->currentHighlighted();
}



} // namespace qt_client
