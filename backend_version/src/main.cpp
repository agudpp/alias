#include <iostream>
#include <vector>
#include <string>
#include <core/types/trie.h>

int
main(void)
{
    // nothing
    std::cout << "This is a test\n";

#define INSERT_T(w) \
    {\
        const bool isNew = t.insert(w);\
        std::cout << "inserting " << w << " and is new: " << isNew << std::endl;\
    }
#define CHECK_T(w)\
        std::cout << "checking " << w << " -> " << t.search(w) << std::endl;

#define SUGGEST(p)\
    {\
        std::vector<std::string> suggests;\
        t.getSuggestions(p, suggests);\
        std::cout << "Suggests for " << p << std::endl;\
        for (auto& su : suggests) {\
            std::cout << "\t" << su << std::endl;\
        }\
    }



    trie t;

    INSERT_T("agu");
    INSERT_T("agu");
    INSERT_T("agustin");
    INSERT_T("fede");
    INSERT_T("agu");
    INSERT_T("locura");
    INSERT_T("algo_mas");
    INSERT_T("agu");
    INSERT_T("locura");

    std::cout << "\n\n";

    CHECK_T("pepe");
    CHECK_T("ag");
    CHECK_T("agus");
    CHECK_T("agu");
    CHECK_T("agustinn");
    CHECK_T("agustin");
    CHECK_T("algo_mas");
    CHECK_T("aagustin");

    std::cout << "\n\n";

    SUGGEST("agustinnn");
    SUGGEST("agustinn");
    SUGGEST("agustin");
    SUGGEST("a");
    SUGGEST("ag");
    SUGGEST("f");
    SUGGEST("fed");
    SUGGEST("l");
    SUGGEST("lo");
    SUGGEST("locura");

    return 0;
}
