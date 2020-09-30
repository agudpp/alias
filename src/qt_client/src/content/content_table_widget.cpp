#include <qt_client/content/content_table_widget.h>

#include <QHeaderView>
#include <QIcon>

#include <toolbox/debug/debug.h>


namespace qt_client {

void
ContentTableWidget::configureTable(void)
{
  // <type> | <element>
  setColumnCount(2);
  setHorizontalHeaderLabels(QStringList{"type", "element"});
  horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

  setFixedHeight(300);
}

ContentTableWidgetItem*
ContentTableWidget::createContentItem(data::Content::ConstPtr content)
{
  ContentTableWidgetItem* item = new ContentTableWidgetItem();
  item->setContent(content);
  return item;
}

QTableWidgetItem*
ContentTableWidget::createTypeItem(data::ContentType content_type)
{
  switch (content_type) {
    case data::ContentType::TEXT: return new QTableWidgetItem(QIcon(":/icons/rsc/clip.svg"), "");
    default:
      LOG_ERROR("Unknown type? maybe we want to default case a question mark icon?");
  }

  return nullptr;
}



ContentTableWidget::ContentTableWidget(QWidget* parent) :
  QTableWidget(parent)
{
  configureTable();
}

ContentTableWidget::~ContentTableWidget()
{

}

void
ContentTableWidget::addContents(const std::vector<data::Content::ConstPtr>& contents)
{
  for (auto& content : contents) {
    const int next_row = rowCount();
    insertRow(next_row);
    setItem(next_row, 0, createTypeItem(content->metadata().type()));
    setItem(next_row, 1, createContentItem(content));
  }
}

void
ContentTableWidget::clear()
{
  clearContents();
  setRowCount(0);
}

ContentTableWidgetItem*
ContentTableWidget::selectNext()
{
  const int curr_row = currentRow();
  const int next_row = hasSelection()
      ? ((curr_row + 1) >= rowCount() ? 0 : curr_row + 1)
      : 0;
  selectRow(next_row);
  return currentSelected();
}

ContentTableWidgetItem*
ContentTableWidget::selectPrev()
{
  const int curr_row = currentRow();
  selectRow(curr_row <= 0 ? rowCount() - 1 : curr_row - 1);
  return currentSelected();
}

bool
ContentTableWidget::hasSelection()
{
  return currentSelected() != nullptr;
}

ContentTableWidgetItem*
ContentTableWidget::currentSelected()
{
  const int current_row = currentRow();
  if (current_row < 0 || current_row > rowCount()) {
    return nullptr;
  }
  return reinterpret_cast<ContentTableWidgetItem*>(item(current_row, 1));
}




} // namespace qt_client

