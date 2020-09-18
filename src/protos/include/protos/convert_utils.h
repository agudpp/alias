#ifndef PROTOS_CONVERTUTILS_H_
#define PROTOS_CONVERTUTILS_H_

#include <data/content.h>
#include <data/metadata.h>
#include <data/tag.h>

#include <content.pb.h>
#include <tag.pb.h>



namespace protos {

/**
 * @brief Utility class to convert from and to protos into data
 */
class ConvertUtils
{
  public:

    static data::Tag
    fromProto(const proto::Tag& proto);
    static data::Metadata
    fromProto(const proto::Metadata& proto);
    static data::Content
    fromProto(const proto::Content& proto);


    static proto::Tag
    toProto(const data::Tag& data);
    static proto::Metadata
    toProto(const data::Metadata& data);
    static proto::Content
    toProto(const data::Content& data);

};


} // namespace protos

#endif // PROTOS_CONVERTUTILS_H_
