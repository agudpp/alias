#ifndef AUTO_FREE_QUEUE_H
#define AUTO_FREE_QUEUE_H

#include <deque>

namespace toolbox {

template<typename T>
class AutoFreeDequeue {
    static_assert (std::is_pointer<T>::value, "AutoFreeQueue should be pointers");
  public:

    inline ~AutoFreeDequeue(void)
    {
      while (!q_.empty()) {
        delete q_.back();
        q_.pop_back();
      }
    }

    inline T& back(void) { return q_.back(); }
    inline const T& back(void) const { return q_.back(); }

    inline void pop_back(void) { q_.pop_back(); }
    inline void pop_front(void) { q_.pop_front(); }

    inline bool empty(void) const { return q_.empty(); }

    inline void push_back(T& v) { q_.push_back(v); }
    inline void push_front(T& v) { q_.push_front(v); }

  private:
    std::deque<T> q_;
};

}

#endif // AUTO_FREE_QUEUE_H
