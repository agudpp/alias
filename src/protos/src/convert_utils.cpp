#include <protos/convert_utils.h>

namespace protos {


data::Tag
ConvertUtils::fromProto(const proto::Tag& proto)
{
  data::Tag result;
  result.setID(toolbox::UID(proto.id()));
  result.setName(proto.name());
  return result;
}

data::Metadata
ConvertUtils::fromProto(const proto::Metadata& proto)
{
  data::Metadata result;
  result.setType(proto.type());
  result.setEncrypted(proto.encrypted());
  return result;
}

data::Content
ConvertUtils::fromProto(const proto::Content& proto)
{
  data::Content result;
  result.setMetadata(fromProto(proto.metadata()));
  result.setID(toolbox::UID(proto.id()));
  result.setData(proto.data());
  for (int i = 0; i < proto.tag_ids_size(); i++) {
    result.addTag(toolbox::UID(proto.tag_ids(i)));
  }
  return result;
}



proto::Tag
ConvertUtils::toProto(const data::Tag& data)
{
  proto::Tag result;
  result.set_id(data.id().toStr());
  result.set_name(data.name());
  return result;
}

proto::Metadata
ConvertUtils::toProto(const data::Metadata& data)
{
  proto::Metadata result;
  result.set_type(data.type());
  result.set_encrypted(data.encrypted());
  return result;
}

proto::Content
ConvertUtils::toProto(const data::Content& data)
{
  proto::Content result;
  *result.mutable_metadata() = toProto(data.metadata());
  result.set_id(data.id().toStr());
  result.set_data(data.data());
  for (auto& tag_id : data.tagIDs()) {
    *result.add_tag_ids() = tag_id.toStr();
  }
  return result;
}


} // namespace protos
