#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <string>
#include <set>
#include <memory>

#include <rapidjson/document.h>

#include <core/types/id_type.h>


class Element {
  public:

    typedef std::shared_ptr<Element> Ptr;
    typedef std::shared_ptr<const Element> ConstPtr;

  public:
    inline Element(void) {}
    inline Element(const core::UID& id);
    virtual ~Element() {}

    /**
     * @brief id returns the id of the element
     * @return the unique id
     */
    inline const core::UID&
    id(void) const;

    /**
     * @brief set the ID
     * @param id the id
     */
    inline void
    setID(const core::UID& id);

    /**
     * @brief addTagID
     * @param id
     */
    inline void
    addTagID(const core::UID& id);

    /**
     * @brief removeTagID
     * @param id
     */
    inline void
    removeTagID(const core::UID& id);

    /**
     * @brief hasTagID
     * @param id
     * @return
     */
    inline bool
    hasTagID(const core::UID& id) const;

    /**
     * @brief Returns the set of associated Tag ids
     * @return the set of associated Tag ids
     */
    inline const std::set<core::UID>&
    tagIDsSet(void) const;

    /**
     * @brief operator ==
     * @param other
     * @return
     */
    inline bool
    operator==(const Element& other) const;

    /**
     * @brief Clones the current element including the id
     * @return should clone the current element
     */
    virtual Element::Ptr
    clone(void) const = 0;

    /**
     * @brief Returns the indexing text associated to this element if any
     * @return
     */
    virtual std::string
    getIndexingTest(void) const = 0;

    /**
     * @brief returns the type of element this is
     * @return the type of element
     */
    virtual std::string
    elementType(void) const = 0;


    /**
     * @brief serialize the current element into the stream
     * @param stream the stream
     * @return true on success false otherwise
     */
    virtual bool
    serialize(std::ostream& stream) const = 0;

    /**
     * @brief deserialize the element from the stream
     * @param stream the stream
     * @return true on success | false otherwise
     */
    virtual bool
    deserialize(std::istream& stream) = 0;

    /**
     * @brief Will fill in this element with the information from the json value
     * @param json_value the json value
     * @return true on success | false otherwise
     */
    virtual bool
    loadFromJsonValue(rapidjson::Value& json_value);

    /**
     * @brief Creates a json value for this element information, associating it to the
     *        provided document
     * @param d the document
     * @return the json value generated
     */
    virtual rapidjson::Value
    toJsonValue(rapidjson::Document& d) const;


  private:
    core::UID id_;
    std::set<core::UID> tag_ids_;
};

////////////////////////////////////////////////////////////////////////////////
inline Element::Element(const core::UID& id) :
    id_(id)
{}

inline const core::UID&
Element::id(void) const
{
    return id_;
}

inline void
Element::setID(const core::UID& id)
{
  id_ = id;
}

inline void
Element::addTagID(const core::UID& id)
{
    tag_ids_.insert(id);
}

void
Element::removeTagID(const core::UID& id)
{
    tag_ids_.erase(id);
}

bool
Element::hasTagID(const core::UID& id) const
{
    return tag_ids_.find(id) != tag_ids_.end();
}

inline const std::set<core::UID>&
Element::tagIDsSet(void) const
{
    return tag_ids_;
}

inline bool
Element::operator==(const Element& other) const
{
  return id_ == other.id_ && tag_ids_ == other.tag_ids_;
}


#endif // ELEMENT_H
