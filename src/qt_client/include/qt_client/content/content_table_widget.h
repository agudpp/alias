#ifndef QT_CLIENT_CONTENT_TABLE_WIDGET_H_
#define QT_CLIENT_CONTENT_TABLE_WIDGET_H_

#include <vector>

#include <QTableWidget>

#include <data/content.h>

#include <qt_client/content/content_table_widget_item.h>

namespace qt_client {


class ContentTableWidget : public QTableWidget
{
  Q_OBJECT

  public:
    ContentTableWidget(QWidget* parent = nullptr);
    virtual ~ContentTableWidget();

    /**
     * @brief Add (append) a list of contents
     * @param contents The list of contents
     */
    void
    addContents(const std::vector<data::Content::ConstPtr>& contents);

    /**
     * @brief Clear the table
     */
    void
    clear();

    /**
     * @brief selectNext
     * @return the next item being selected or null if none
     */
    ContentTableWidgetItem*
    selectNext();

    /**
     * @brief selectPrev
     * @return the prev item being selected or null if none
     */
    ContentTableWidgetItem*
    selectPrev();

    /**
     * @brief hasSelection
     * @return true if we have, false otherwise
     */
    bool
    hasSelection();

    /**
     * @brief currentSelected
     * @return current selected item or null if none
     */
    ContentTableWidgetItem*
    currentSelected();

    /**
     * @brief Override function hack to be able to show icons items centered
     * @return
     */
    QStyleOptionViewItem
    viewOptions() const override;


  private:

    /**
     * @brief configureTable
     */
    void
    configureTable(void);

    /**
     * @brief Creates an Item from a content
     * @param content The content
     * @return the allocated item with the given content. Caller should handle memory
     */
    ContentTableWidgetItem*
    createContentItem(data::Content::ConstPtr content);

    /**
     * @brief Creates a item based on the type (for the type column on the table)
     * @param content_type The content to know what type should we build
     * @return the allocated table item, caller should handle memory
     */
    QTableWidgetItem*
    createTypeItem(data::ContentType content_type);

};

} // namespace qt_client

#endif // QT_CLIENT_CONTENT_TABLE_WIDGET_H_
