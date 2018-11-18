#include "elementmanager.h"

#include <core/debug/Debug.h>


////////////////////////////////////////////////////////////////////////////////
ElementManager::ElementManager()
{
}

////////////////////////////////////////////////////////////////////////////////
ElementManager::~ElementManager()
{
}


////////////////////////////////////////////////////////////////////////////////

bool
ElementManager::removeElement(const core::UID& id)
{
  return elements_map_.erase(id) > 0;
}

bool
ElementManager::addElement(Element::Ptr e)
{
  if (hasElement(e)) {
    debugERROR("We are trying to add a element that we already have %s", e->id().toStr().c_str());
    return false;
  }
  elements_map_.insert(std::make_pair(e->id(), e));
  return true;
}


void
ElementManager::getAllElements(std::vector<const Element*>& elements)
{
  elements.clear();
  for (auto it = elements_map_.begin(); it != elements_map_.end(); ++it) {
    elements.push_back(it->second.get());
  }
}
