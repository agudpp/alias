#ifndef TOOLBOX_STD_UTILS_H_
#define TOOLBOX_STD_UTILS_H_

#include <algorithm>
#include <set>
#include <vector>

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

    /**
     * @brief Makes a vector unique with its elements and keep the ordering
     * @param v The vector to make unique
     * @return the copy of the vector with the unique elements
     */
    template<typename T>
    static std::vector<T> stableUnique(const std::vector<T>& v)
    {
      std::set<T> refs;
      std::vector<T> result;
      result.reserve(v.size());
      for (auto& e : v) {
        if (refs.count(e) == 0) {
          refs.insert(e);
          result.push_back(e);
        }
      }
      return result;
    }

    /**
     * @brief Convertion function from vector to set
     */
    template<typename T>
    static std::set<T> vectorToSet(const std::vector<T>& vec)
    {
      return std::set<T>(vec.begin(), vec.end());
    }

    /**
     * @brief Convertion function from set to vector
     */
    template<typename T>
    static std::vector<T> setToVector(const std::set<T>& s)
    {
      return std::vector<T>(s.begin(), s.end());
    }

    /**
     * @brief Check if a vector contains an element
     */
    template<typename T>
    static bool contains(const std::vector<T>& v, const T& element)
    {
      return std::find(v.begin(), v.end(), element) != v.end();
    }

    template<typename T>
    static void remove(std::vector<T>& v, const T& element)
    {
      v.erase(std::remove(v.begin(), v.end(), element), v.end());
    }






};

} // namespace toolbox

#endif // TOOLBOX_STD_UTILS_H_
