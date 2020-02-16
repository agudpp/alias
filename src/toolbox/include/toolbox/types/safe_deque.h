#ifndef TOOLBOX_SAFE_DEQUE_H_
#define TOOLBOX_SAFE_DEQUE_H_

#include <deque>
#include <mutex>
#include <condition_variable>

namespace toolbox {

// A threadsafe-queue.
template <class T>
class SafeDeque
{
public:
  SafeDeque(void) = default;
  ~SafeDeque(void) {}

  /**
   * @brief Push an element to the back of the deque
   * @param t the element
   */
  inline void
  push_back(const T& t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push_back(t);
    c.notify_one();
  }

  /**
   * @brief Push an element to the fron of the deque
   * @param t the element
   */
  inline void
  push_front(const T& t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push_front(t);
    c.notify_one();
  }

  /**
   * @brief Pops and returns the front element of the queue. Will get blocked till there
   *        is an element on the queue.
   * @return the front element of the queue
   */
  inline T
  pop_front(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.front();
    q.pop_front();
    return val;
  }
  inline T
  pop_back(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.back();
    q.pop_back();
    return val;
  }

private:
  std::deque<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};

}

#endif // TOOLBOX_SAFE_DEQUE_H_
