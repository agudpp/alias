#ifndef TOOLBOX_MAP_UTILS_H_
#define TOOLBOX_MAP_UTILS_H_

namespace toolbox {

namespace map_utils {

template <typename ContainerT, typename PredicateT>
void erase_if(ContainerT& items, const PredicateT& predicate) {
  for (auto it = items.begin(); it != items.end();) {
    if (predicate(*it)) {
      it = items.erase(it);
    } else {
      ++it;
    }
  }
}

}


}


#endif // TOOLBOX_MAP_UTILS_H_
