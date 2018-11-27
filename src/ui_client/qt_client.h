#ifndef QT_CLIENT_H
#define QT_CLIENT_H

#include <service_api/serviceapi.h>


class QTClient
{
  public:

    static int
    execute(int argc, char *argv[], ServiceAPI* service_api);
};

#endif // QT_CLIENT_H
