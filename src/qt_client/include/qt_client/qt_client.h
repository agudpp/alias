#ifndef QT_CLIENT_QT_CLIENT_H_
#define QT_CLIENT_QT_CLIENT_H_

#include <service/service_api.h>

namespace qt_client {


class QTClient
{
  public:

    static int
    execute(int argc, char *argv[], service::ServiceAPI::Ptr service_api);
};

} // namespace qt_client

#endif // QT_CLIENT_QT_CLIENT_H_
