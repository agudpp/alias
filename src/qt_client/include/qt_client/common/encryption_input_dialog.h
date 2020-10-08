#ifndef QT_CLIENT_ENCRYPTION_INPUT_DIALOG_H_
#define QT_CLIENT_ENCRYPTION_INPUT_DIALOG_H_


#include <QInputDialog>
#include <QString>


namespace qt_client {


class EncryptionInputDialog : public QInputDialog
{
  Q_OBJECT

  public:
    EncryptionInputDialog(QWidget* parent = nullptr);
    virtual ~EncryptionInputDialog() = default;




};

} // namespace qt_client

#endif // QT_CLIENT_ENCRYPTION_INPUT_DIALOG_H_
