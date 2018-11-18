#ifndef ELEMENTMANAGER_H
#define ELEMENTMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include <core/types/id_type.h>

#include <elements/element.h>


class ElementManager
{
public:
  ElementManager();
  ~ElementManager();

  bool
  removeElement(const core::UID& id);
  inline bool
  removeElement(const Element::Ptr& element);
  bool
  addElement(Element::Ptr element);

  inline bool
  hasElement(const core::UID& id) const;
  inline bool
  hasElement(const Element::Ptr& element) const;
  inline Element::ConstPtr
  getElement(const core::UID& id) const;
  inline Element::Ptr
  getElement(const core::UID& id);
  inline std::vector<Element::ConstPtr>
  getElements(const std::set<core::UID>& ids) const;
  inline std::vector<Element::Ptr>
  getElements(const std::set<core::UID>& ids);

  ///
  /// \brief getAllElements returns all the elements, should be an iterator
  /// \param elements
  ///
  void
  getAllElements(std::vector<const Element*>& elements);


private:
  typedef std::unordered_map<core::UID, Element::Ptr> ElementHash;

private:
  ElementHash elements_map_;

};




////////////////////////////////////////////////////////////////////////////////

inline bool
ElementManager::removeElement(const Element::Ptr& element)
{
  if (element.get() == nullptr) {
    return false;
  }
  return removeElement(element->id());
}


inline bool
ElementManager::hasElement(const core::UID& id) const
{
    return elements_map_.find(id) != elements_map_.end();
}
inline bool
ElementManager::hasElement(const Element::Ptr& element) const
{
  return element.get() == nullptr ? false : hasElement(element->id());
}


inline Element::ConstPtr
ElementManager::getElement(const core::UID& id) const
{
    auto it = elements_map_.find(id);
    return (it == elements_map_.end()) ? Element::ConstPtr() : it->second;
}
inline Element::Ptr
ElementManager::getElement(const core::UID& id)
{
    auto it = elements_map_.find(id);
    return (it == elements_map_.end()) ? Element::Ptr() : it->second;
}

inline std::vector<Element::ConstPtr>
ElementManager::getElements(const std::set<core::UID>& ids) const
{
  std::vector<Element::ConstPtr> result;
  for (const core::UID& id : ids) {
    Element::ConstPtr elem = getElement(id);
    if (elem.get() != nullptr) {
      result.push_back(elem);
    }
  }
  return result;
}

inline std::vector<Element::Ptr>
ElementManager::getElements(const std::set<core::UID>& ids)
{
  std::vector<Element::Ptr> result;
  for (const core::UID& id : ids) {
    Element::Ptr elem = getElement(id);
    if (elem.get() != nullptr) {
      result.push_back(elem);
    }
  }
  return result;
}

#endif // ELEMENTMANAGER_H
