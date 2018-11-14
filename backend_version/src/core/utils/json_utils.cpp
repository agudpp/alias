#include <core/utils/json_utils.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace core {


rapidjson::Document
JsonUtils::fromStream(std::istream& stream)
{
  rapidjson::Document d;
  rapidjson::IStreamWrapper isw(stream);
  d.ParseStream(isw);
  return d;
}

std::string
JsonUtils::toString(rapidjson::Document& d)
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
