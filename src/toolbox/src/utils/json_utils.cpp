#include <toolbox/utils/json_utils.h>

#include <toolbox/rapidjson/istreamwrapper.h>
#include <toolbox/rapidjson/ostreamwrapper.h>
#include <toolbox/rapidjson/stringbuffer.h>
#include <toolbox/rapidjson/writer.h>

namespace toolbox {


rapidjson::Document
JsonUtils::fromStream(std::istream& stream)
{
  rapidjson::Document d;
  rapidjson::IStreamWrapper isw(stream);
  d.ParseStream(isw);
  return d;
}

std::string
JsonUtils::toString(const rapidjson::Document& d)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    return buffer.GetString();
}

void
JsonUtils::toStream(rapidjson::Document& d, std::ostream& stream)
{
  rapidjson::OStreamWrapper osw(stream);
  rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
  d.Accept(writer);
}


}
