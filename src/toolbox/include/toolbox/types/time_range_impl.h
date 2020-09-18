#ifndef TOOLBOX_TIME_RANGE_IMPL_H_
#define TOOLBOX_TIME_RANGE_IMPL_H_


namespace toolbox {

inline
TimeRange::TimeRange(const Time& start, const Time& end) :
  start_(start)
, end_(end)
{}

inline bool
TimeRange::isInRange(const Time& time) const
{
  return time >= start_ && time <= end_;
}

inline bool
TimeRange::isDefined(void) const
{
  return start_.isDefined() && end_.isDefined();
}


static inline std::ostream&
operator<<(std::ostream& out_stream, const TimeRange& time_range)
{
  out_stream << "TimeRange: {start: " << time_range.start() << ", end: " << time_range.end()
             << ", duration_secs = " << time_range.start().durationSecsTill(time_range.end()) << "}";
  return out_stream;
}

}

#endif // TOOLBOX_TIME_RANGE_IMPL_H_
