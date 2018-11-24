#ifndef REUSABLE_WIDGET_H
#define REUSABLE_WIDGET_H

#include <deque>


template<typename T>
class ReusableWidget
{
  public:
    static std::deque<ReusableWidget<T>*> QUEUE;
  public:
    ReusableWidget(){}
    virtual ~ReusableWidget() {}

    virtual void
    cleanObject(void)
    {
    }

    inline void
    freeObject(void)
    {
      // should be a widget
      static_cast<T*>(this)->setParent(nullptr);
      QUEUE.push_back(this);
    }

    static T*
    getOne(void)
    {
      if (QUEUE.empty()) {
        return new T;
      }
      auto obj = QUEUE.front();
      QUEUE.pop_front();
      return dynamic_cast<T*>(obj);
    }

    static void
    deleteAll(void)
    {
      while (!QUEUE.empty()) {
        delete QUEUE.front();
        QUEUE.pop_front();
      }
    }
};

template<typename T>
std::deque<ReusableWidget<T>*> ReusableWidget<T>::QUEUE;

#endif // REUSABLE_WIDGET_H
