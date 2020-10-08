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
     * @param start_hidden
     * @return
     */
    static int
    execute(int argc,
            char *argv[],
            service::ServiceAPI::Ptr service_api,
            const toolbox::Config& config,
            bool start_hidden = true);
};

} // namespace qt_client

#endif // QT_CLIENT_QT_CLIENT_H_
