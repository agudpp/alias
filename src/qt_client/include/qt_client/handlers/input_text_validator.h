#ifndef QT_CLIENT_INPUT_TEXT_VALIDATOR_H_
#define QT_CLIENT_INPUT_TEXT_VALIDATOR_H_

#include <QString>
#include <QDebug>
#include <QValidator>

namespace qt_client {

class InputTextValidator : public QValidator
{
  private:
    static const QString NOT_ACCEPTED_CHARS;
  public:
    InputTextValidator(QObject* parent = nullptr) : QValidator(parent) {}
    ~InputTextValidator() override {}

    virtual QValidator::State
    validate(QString &input, int &pos) const override
    {
      // TODO optimize this
      for (const QChar& invalid_char : NOT_ACCEPTED_CHARS) {
        if (input.indexOf(invalid_char) >= 0) {
          return QValidator::State::Invalid;
        }
      }
      return QValidator::State::Acceptable;
    }
};



} // namespace qt_client

#endif // QT_CLIENT_INPUT_TEXT_VALIDATOR_H_
