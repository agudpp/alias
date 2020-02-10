#include <ui_client/elements/element_table_widget.h>

#include <QHeaderView>
#include <QIcon>
#include <QtDebug>

#include <core/debug/Debug.h>


Q_DECLARE_METATYPE(const ElementUIBase*)



void
ElementTableWidget::configureTable(void)
{
  // <type> | <element>
  table_->setColumnCount(2);
  table_->setHorizontalHeaderLabels(QStringList{"type", "element"});
  table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

  table_->setFixedHeight(300);

}

QTableWidgetItem*
ElementTableWidget::getTypeItem(const ElementUIBase* element) const
{
  ASSERT_PTR(element);
  // todo add icons here later
  return new QTableWidgetItem(QIcon(":/icons/rsc/clip.svg"), "");
}

ElementTableWidget::ElementTableWidget() :
  table_(new QTableWidget())
{
  configureTable();
}

ElementTableWidget::~ElementTableWidget()
{
  delete table_;
  table_ = nullptr;
}

void
ElementTableWidget::addElement(const ElementUIBase* element)
{
  ASSERT_PTR(element);
  QTableWidgetItem* elem_item = element->tableRepresentation();
  ASSERT_PTR(elem_item);

  QVariant user_ptr;
  user_ptr.setValue(element);
  elem_item->setData(Qt::UserRole, user_ptr);
  QTableWidgetItem* type_item = getTypeItem(element);

  const int next_row = table_->rowCount();
  table_->insertRow(next_row);
  table_->setItem(next_row, 0, type_item);
  table_->setItem(next_row, 1, elem_item);

  qDebug() << "adding element: " << element->elementType();
}

void
ElementTableWidget::clear(void)
{
  table_->clearContents();
  table_->setRowCount(0);
}

void
ElementTableWidget::selectNext(void)
{
  const int curr_row = table_->currentRow();
  table_->selectRow((curr_row + 1) >= table_->rowCount() ? 0 : curr_row + 1);
}

void
ElementTableWidget::selectPrevious(void)
{
  const int curr_row = table_->currentRow();
  table_->selectRow(curr_row <= 0 ? table_->rowCount() - 1 : curr_row - 1);
}

void
ElementTableWidget::unselectCurrent(void)
{
  table_->selectRow(-1);
}

bool
ElementTableWidget::hasSelected(void) const
{
  const int curr_row = table_->currentRow();
  return  curr_row >= 0 && curr_row < table_->rowCount();
}

const ElementUIBase*
ElementTableWidget::selected(void) const
{
  if (!hasSelected()) {
    return nullptr;
  }
  const QTableWidgetItem* item = table_->item(table_->currentRow(), 1);
  ASSERT_PTR(item);
  const ElementUIBase* ui_element = item->data(Qt::UserRole).value<const ElementUIBase*>();
  ASSERT_PTR(ui_element);
  return ui_element;
}
