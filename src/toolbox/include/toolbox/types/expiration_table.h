#ifndef TOOLBOX_EXPIRATION_TABLE_H_
#define TOOLBOX_EXPIRATION_TABLE_H_

#include <unordered_map>

#include <toolbox/types/time.h>
#include <toolbox/utils/map_utils.h>

namespace toolbox {


template<typename T>
class ExpirationTable {
  public:

    typedef std::unordered_map<T, Time> TToTimeMap;

  public:
    ExpirationTable(const long persist_time_secs = 120) :
      persist_time_secs_(persist_time_secs)
    {}

    /**
     * @brief Erase from the table all the items (T, time) that match the predicate
     * @param predicate the predicate used to erase the elements
     */
    template<typename Predicate>
    inline void
    eraseIf(const Predicate& predicate)
    {
      map_utils::erase_if(time_map_, predicate);
    }

    /**
     * @brief Add or updates a new element on the table
     * @param elem the element to be added
     */
    inline void
    addOrUpdate(const T& elem)
    {
      cleanExpired();
      time_map_[elem] = getExpirationTime();
    }

    /**
     * @brief Removes an element from the table
     * @param elem the element to remove
     */
    inline void
    remove(const T& elem)
    {
      cleanExpired();
      time_map_.erase(elem);
    }

    /**
     * @brief Checks if the element expired or not (i.e. is present on the table or not)
     * @param elem the element
     * @return true if the element expired or is not on the table, false otherwise
     */
    inline bool
    expired(const T& elem) const
    {
      cleanExpired();
      return time_map_.find(elem) == time_map_.end();
    }

    inline typename TToTimeMap::iterator
    begin(void)
    {
      return time_map_.begin();
    }
    inline typename TToTimeMap::const_iterator
    begin(void) const
    {
      return time_map_.begin();
    }
    inline typename TToTimeMap::iterator
    end(void)
    {
      return time_map_.end();
    }
    inline typename TToTimeMap::const_iterator
    end(void) const
    {
      return time_map_.end();
    }

  private:

    /**
     * @brief Clean expired elements if any
     */
    inline void
    cleanExpired(void) const
    {
      const Time now = toolbox::Time::now();
      map_utils::erase_if(time_map_, [&now](const std::pair<T, Time>& pair) {
        return pair.second < now;
      });
    }

    /**
     * @brief Get the next expiration time from now
     * @return the expiration time
     */
    inline toolbox::Time
    getExpirationTime(void) const
    {
      return toolbox::Time::now() + toolbox::Time::Seconds(persist_time_secs_);
    }


  private:
    mutable TToTimeMap time_map_;
    long persist_time_secs_;
};



}


#endif // TOOLBOX_EXPIRATION_TABLE_H_
