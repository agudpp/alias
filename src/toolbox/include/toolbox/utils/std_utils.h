#ifndef TOOLBOX_STD_UTILS_H_
#define TOOLBOX_STD_UTILS_H_

#include <set>

namespace toolbox {


class StdUtils {
  public:

    /**
     * @brief Get the union of 2 sets and returns it
     * @param a The first set
     * @param b The second set
     * @return the union of a and b (a | b)
     */
    template<typename T>
    static std::set<T> getUnion(const std::set<T>& a, const std::set<T>& b)
    {
      std::set<T> result = a;
      result.insert(b.begin(), b.end());
      return result;
    }

};

} // namespace toolbox

#endif // TOOLBOX_STD_UTILS_H_
