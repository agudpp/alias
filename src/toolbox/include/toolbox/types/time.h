#ifndef TOOLBOX_TIME_H_
#define TOOLBOX_TIME_H_

#include <iostream>
#include <chrono>
#include <cstdint>

namespace toolbox {



class Time {
  public:
    typedef std::chrono::hours Hours;
    typedef std::chrono::minutes Minutes;
    typedef std::chrono::seconds Seconds;
    typedef std::chrono::milliseconds Milliseconds;
  public:
    Time(void) {}
    explicit Time(const std::chrono::system_clock::time_point& time_point) : time_point_(time_point) {}
    explicit Time(const time_t& time) : time_point_(std::chrono::system_clock::from_time_t(time)) {}

    static inline Time now(void) { return Time(std::chrono::system_clock::now()); }

    static inline Time
    fromSecsFromEpoch(const std::int64_t& seconds_from_epoch)
    {
      const std::chrono::system_clock::duration duration_sesc = Seconds(seconds_from_epoch);
      return Time(std::chrono::system_clock::time_point(duration_sesc));
    }

    template<typename DurationT>
    inline void
    addDuration(const DurationT& duration)
    {
      const std::chrono::system_clock::duration duration_val = duration;
      time_point_ += duration_val;
    }

    inline bool operator<(const Time& other) const { return time_point_ < other.time_point_; }
    inline bool operator<=(const Time& other) const { return time_point_ <= other.time_point_; }
    inline bool operator>(const Time& other) const { return time_point_ > other.time_point_; }
    inline bool operator>=(const Time& other) const { return time_point_ >= other.time_point_; }
    inline bool operator==(const Time& other) const { return time_point_ == other.time_point_; }

    inline std::int64_t durationSecsTill(const Time& other) const { return std::chrono::duration_cast<Seconds>(other.time_point_ - time_point_).count(); }

    // TODO(aguperez) define if there is a better way to check this
    // TODO(aguperez) THIS IS NOT GOING TO WORK
    inline bool isDefined(void) const { return std::chrono::duration<double>(time_point_.time_since_epoch()).count() != 0.; }

    inline const std::chrono::system_clock::time_point&
    timePoint(void) const
    {
      return time_point_;
    }

    inline const std::time_t
    toTimeT(void) const
    {
      return std::chrono::system_clock::to_time_t(time_point_);
    }

    inline std::int64_t
    secondsFromEpoch(void) const
    {
      return std::chrono::duration_cast<Seconds>(time_point_.time_since_epoch()).count();
    }

  private:
    std::chrono::system_clock::time_point time_point_;
};

template<typename Duration>
inline Time
operator+(const Duration& duration, const Time& time)
{
  return Time(time.timePoint() + duration);
}

template<typename Duration>
inline Time
operator+(const Time& time, const Duration& duration)
{
  return Time(time.timePoint() + duration);
}

template<typename Duration>
inline Time
operator-(const Duration& duration, const Time& time)
{
  return Time(time.timePoint() - duration);
}

template<typename Duration>
inline Time
operator-(const Time& time, const Duration& duration)
{
  return Time(time.timePoint() - duration);
}


static inline std::ostream&
operator<<(std::ostream& out_stream, const Time& time)
{
  out_stream << "Time: {" << time.secondsFromEpoch() << "}";
  return out_stream;
}


}


#endif // TOOLBOX_TIME_H_
