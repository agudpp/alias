#ifndef TOOLBOX_TIME_RANGE_H_
#define TOOLBOX_TIME_RANGE_H_

#include <chrono>
#include <iostream>

#include <toolbox/types/time.h>

namespace toolbox {


class TimeRange {
  public:
    TimeRange(void) = default;

    inline TimeRange(const Time& start, const Time& end);

    inline bool
    isInRange(const Time& time) const;

    /**
     * @brief Check if it is defined or not
     * @return true if it is defined, false otherwise
     */
    inline bool
    isDefined(void) const;

    /**
     * @brief Return the start time range
     * @return the start time range
     */
    inline const Time&
    start(void) const
    {
      return start_;
    }

    /**
     * @brief Return the end time range
     * @return the end time range
     */
    inline const Time&
    end(void) const
    {
      return end_;
    }


  private:
    Time start_;
    Time end_;
};

}

#include <toolbox/types/time_range_impl.h>

#endif // TOOLBOX_TIME_RANGE_H_
