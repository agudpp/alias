#ifndef QT_CLIENT_QT_CLIENT_H_
#define QT_CLIENT_QT_CLIENT_H_

#include <toolbox/config/config.h>

#include <service/service_api.h>

namespace qt_client {


class QTClient
{
  public:

    /**
     * @brief execute
     * @param argc
     * @param argv
     * @param service_api
     * @param config
     * @return
     */
    static int
    execute(int argc,
            char *argv[],
            service::ServiceAPI::Ptr service_api,
            const toolbox::Config& config);
};

} // namespace qt_client

#endif // QT_CLIENT_QT_CLIENT_H_
