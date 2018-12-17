#ifndef BACKEND_CONFIG_H
#define BACKEND_CONFIG_H

#include <string>

#include <rapidjson/document.h>


struct BackendConfig {
    /**
     * @brief the database path
     */
    std::string db_path;
    // TODO: add the key mapping here
};

#endif // BACKEND_CONFIG_H
