#ifndef TOOLBOX_CONFIG_H_
#define TOOLBOX_CONFIG_H_

#include <iostream>

#include <toolbox/rapidjson/document.h>


namespace toolbox {


class Config
{
  public:
    Config();
    Config(const Config& other);
    Config& operator=(const Config& other);

    bool
    parse(std::istream& stream);
    bool
    parseFromFilePath(const std::string& file_path);

    bool
    getValue(const std::string& key, int& value) const;
    bool
    getValue(const std::string& key, float& value) const;
    bool
    getValue(const std::string& key, std::string& value) const;

    bool
    hasKey(const std::string& key) const;

    bool
    getConfig(const std::string& key, Config& config) const;


    void
    dumpJson(void) const;


  private:
    Config(const rapidjson::Document& json_doc);


  private:
    rapidjson::Document json_doc_;
};

}


#endif // TOOLBOX_CONFIG_H_

