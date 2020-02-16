#ifndef TIME_STAMP_H
#define TIME_STAMP_H

/*******************************************************************************
 HAPPEN 2016
*******************************************************************************/

#include <ctime>

namespace com {

class TimeStamp
{
    ///
    /// Time stamps for packets time tracking.
    ///

public:

    /// @brief ctor
    TimeStamp(void);

    /// @brief dtor
    virtual ~TimeStamp(void);

    ///
    /// @brief Update internal state to current time
    ///
    bool
    updateCurrentTime(void);

    ///
    /// @brief Returns nanosecond? difference between this and @other
    ///
    long
    operator-(const TimeStamp &other);

    ///
    /// @brief  Return the nanoseconds? passed since last update of the
    ///         timestamp.
    ///
    long
    getTimeSinceLastUpdate(void);


    ///
    /// @brief Get the time stamp in nanoseconds?
    ///
    long
    getTimeStamp(void) const;

private:

    struct timespec m_time;

};

////////////////////////////////////////////////////////////////////////////////

TimeStamp::TimeStamp()
{
    this->updateCurrentTime();
}

////////////////////////////////////////////////////////////////////////////////

TimeStamp::~TimeStamp()
{
}

////////////////////////////////////////////////////////////////////////////////

bool
TimeStamp::updateCurrentTime(void)
{
    int res = 0;
    res = clock_gettime(CLOCK_REALTIME, &m_time);
    if(res){
        perror("Error updating time stamp.");
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

long
TimeStamp::operator-(const TimeStamp &other)
{
    return m_time.tv_nsec - other.getTimeStamp();
}

////////////////////////////////////////////////////////////////////////////////

long
TimeStamp::getTimeSinceLastUpdate(void)
{
    struct timespec time;
    int res = 0;
    res = clock_gettime(CLOCK_REALTIME, &time);
    if(res){
        perror("Error getting time stamp difference.");
        return -1;
    }
    return time.tv_nsec - m_time.tv_nsec;
}

////////////////////////////////////////////////////////////////////////////////

inline long
TimeStamp::getTimeStamp(void) const
{
    return m_time.tv_nsec;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace com

#endif // TIME_STAMP_H
