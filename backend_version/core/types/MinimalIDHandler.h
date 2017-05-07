#ifndef MINIMALIDHANDLER_H
#define MINIMALIDHANDLER_H

#include <set>

#include <core/debug/Debug.h>


namespace core {

///
/// \brief This class will be used to maintain a track of id's using always
///
template<typename ID_TYPE, unsigned int MAX_SIZE = 128>
class MinimalIDHandler {
public:
    inline MinimalIDHandler();

    ///
    /// \brief Pops a new id to be used
    /// \return the new unique id
    ///
    inline ID_TYPE
    newID(void);

    ///
    /// \brief Push back an ID that is not used anymore
    /// \param id   the ID that is not used anymore (and will be reused)
    ///
    inline void
    unusedID(ID_TYPE id);

    ///
    /// \brief This method will mark the id as used so it is not returned when
    ///        calling "newID".
    /// \param id   The id to invalidate.
    ///
    inline void
    markAsUsed(ID_TYPE id);

    ///
    /// \brief Reset all the ids, this will assume that all the current ids will
    ///        not be used anymore (so don't call unusedID after calling reset)
    ///
    inline void
    reset(void);

private:
    //core::StackVector<ID_TYPE, MAX_SIZE> m_ids;
    //int m_index;
    // we will change the implementation to use a std::set instead
    // since we add a "markAsUsed" method and we need to be more efficient now
    std::set<ID_TYPE> m_ids;
};











////////////////////////////////////////////////////////////////////////////////
// Inline stuff
//

template<typename ID_TYPE, unsigned int MAX_SIZE>
inline MinimalIDHandler<ID_TYPE, MAX_SIZE>::MinimalIDHandler()
{
    reset();
}

template<typename ID_TYPE, unsigned int MAX_SIZE>
inline ID_TYPE
MinimalIDHandler<ID_TYPE, MAX_SIZE>::newID(void)
{
    ASSERT(!m_ids.empty() && "The size wasn't enough?");
    const ID_TYPE elem = *m_ids.begin();
    m_ids.erase(m_ids.begin());
    return elem;
}

template<typename ID_TYPE, unsigned int MAX_SIZE>
inline void
MinimalIDHandler<ID_TYPE, MAX_SIZE>::unusedID(ID_TYPE id)
{
    m_ids.insert(id);
}

template<typename ID_TYPE, unsigned int MAX_SIZE>
inline void
MinimalIDHandler<ID_TYPE, MAX_SIZE>::markAsUsed(ID_TYPE id)
{
    m_ids.erase(id);
}


template<typename ID_TYPE, unsigned int MAX_SIZE>
inline void
MinimalIDHandler<ID_TYPE, MAX_SIZE>::reset(void)
{
    for (unsigned int i = 0; i < MAX_SIZE; ++i) {
        m_ids.insert(static_cast<ID_TYPE>(i));
    }
}



}

#endif // MINIMALIDHANDLER_H
