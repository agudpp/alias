#ifndef TOOLBOX_RANDOM_H_
#define TOOLBOX_RANDOM_H_

#include <ctime>
#include <cstdlib>

namespace toolbox {

class Random {
  public:

    static inline void
    init(void)
    {
      std::srand(std::time(nullptr));
    }

    /**
     * @brief Generate a random integer number
     * @return the random integer number
     */
    static inline int
    getRandom(void)
    {
      return std::rand();
    }

    static inline char
    getRandomChar(void)
    {
      return static_cast<char>(std::rand());
    }

};

}

#endif // TOOLBOX_RANDOM_H_
