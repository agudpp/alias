#ifndef ELEMENTMANAGER_H
#define ELEMENTMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include <core/types/id_type.h>

#include "element.h"


class ElementManager
{
public:
  typedef std::shared_ptr<Element> ElementPtr;

public:
  ElementManager();
  ~ElementManager();

  bool
  removeElement(const core::UID& id);
  inline bool
  removeElement(const ElementPtr& element);
  bool
  addElement(ElementPtr element);

  inline bool
  hasElement(const core::UID& id) const;
  inline bool
  hasElement(const ElementPtr& element) const;
  inline const Element*
  getElement(const core::UID& id) const;
  inline Element*
  getElement(const core::UID& id);

  ///
  /// \brief getAllElements returns all the elements, should be an iterator
  /// \param elements
  ///
  void
  getAllElements(std::vector<const Element*>& elements);

private:
  typedef std::unordered_map<core::UID, ElementPtr> ElementHash;

private:
  ElementHash elements_map_;

};




////////////////////////////////////////////////////////////////////////////////

inline bool
ElementManager::removeElement(const ElementPtr& element)
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
ElementManager::hasElement(const ElementPtr& element) const
{
  return element.get() == nullptr ? false : hasElement(element->id());
}


inline const Element*
ElementManager::getElement(const core::UID& id) const
{
    auto it = elements_map_.find(id);
    return (it == elements_map_.end()) ? nullptr : it->second.get();
}
inline Element*
ElementManager::getElement(const core::UID& id)
{
    auto it = elements_map_.find(id);
    return (it == elements_map_.end()) ? nullptr : it->second.get();
}


#endif // ELEMENTMANAGER_H
