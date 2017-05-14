#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <memory>

#include <core/types/trie.h>
#include <core/logger/LoggerManager.h>
#include <core/logger/Loggers.h>
#include <core/debug/Debug.h>

#include <tags/tagmanager.h>
#include <tags/tag.h>
#include <elements/element.h>
#include <elements/elementmanager.h>
#include "serviceapi.h"
#include "server.h"


typedef std::shared_ptr<core::Logger> LoggerPtr;

////////////////////////////////////////////////////////////////////////////////
static void
configureLoggingSystem(std::vector<LoggerPtr>& loggers)
{
    loggers.clear();
    loggers.push_back(LoggerPtr(new core::ConsoleLogger));
    for (unsigned int i = 0; i < loggers.size(); ++i) {
        core::LoggerManager::instance().addLogger(loggers[i].get());
    }
    core::LoggerManager::instance().configureLevel(core::LogLevel::LL_0);
}


static void
loadData(TagManager& tm, ElementManager& em)
{
#define ASSOCIATE_ELEM(t, e) \
    t->addElementID(e->id());\
    e->addTagID(t->id());
#define CREATE_A(tT, eT) \
    {\
        tag* tptr = tm.createTag(tT);\
        element* eptr = em.createElement(eT);\
        ASSOCIATE_ELEM(tptr, eptr);\
    }

    CREATE_A("bank_account", "12312312");
    CREATE_A("name", "agustin");
    CREATE_A("last", "perez");
    CREATE_A("friend", "fede");
    CREATE_A("friends", "fede gringo loco");
    CREATE_A("france", "pais en algun lado");
    CREATE_A("fr", "fr? no se");
    CREATE_A("names", "Agustin daniel");
    CREATE_A("names-and-last", "Agu pere pa");
}

static std::string
toString(const std::vector<std::string>& d)
{
    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < d.size(); ++i) {
        ss << d[i];
        if (i != (d.size()-1)) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}
static std::string
toString(const std::set<const tag*>& ts)
{
    std::stringstream ss;
    ss << "{";
    for (const tag* t : ts) {
        ss << t->text() << ", ";
    }
    ss << "}";
    return ss.str();
}
static std::string
toString(const std::set<const element*>& es)
{
    std::stringstream ss;
    ss << "{";
    for (const element* t : es) {
        ss << t->text() << ", ";
    }
    ss << "}";
    return ss.str();
}

static void
perfSearch(const ServiceAPI::SearchOptions& so, ServiceAPI& sa)
{
    ServiceAPI::SearchResult sr;
    if (!sa.search(so, sr)) {
        std::cerr << "Error performing the search\n";
        return;
    }

    std::cout << "------------------------------------------------------\n";
    std::cout << "Search: " << so.query << "\n"
              << "tags: " << toString(so.tags) << "\n\n";

    std::cout << "results: \n";
    std::cout << "Matched tags: " << toString(sr.matchedTags) << "\n"
              << "expanded tags: " << toString(sr.expandedTags) << "\n"
              << "expanded results: \n";
    for (auto it = sr.expResults.begin(); it != sr.expResults.end(); ++it) {
        std::cout << "\t";
        if (it->first) {
            std::cout << it->first->text();
        }
        std::cout << ": " << toString(it->second) << "\n";
//        std::cout << "\t" << it->first ? it->first->text() : " ";
//        std::cout << ": " << toString(it->second) << "\n";
    }

}


int
main(void)
{
    // init logging system
    std::vector<LoggerPtr> loggers;
    configureLoggingSystem(loggers);

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


    TagManager tm;
    ElementManager em;
    ServiceAPI sa;
    ServiceAPI::MainData md;
    md.elemMngr = &em;
    md.tagMngr = &tm;

    loadData(tm, em);
    if (!sa.init(md)) {
        std::cerr << "Error initializing the service\n";
        return -1;
    }

    ServiceAPI::SearchOptions so;
    so.query = "n";
    so.tags.clear();
    perfSearch(so, sa);

    so.query = "name";
    so.tags.clear();
    perfSearch(so, sa);

    so.query = "name";
    so.tags = {"fr", "last", "sa"};
    perfSearch(so, sa);

    so.query = "fri";
    so.tags.clear();
    perfSearch(so, sa);

    so.query = "friend";
    so.tags.clear();
    perfSearch(so, sa);

    so.query = "friends";
    so.tags.clear();
    perfSearch(so, sa);


    Server server(&sa);
    server.start();


    return 0;
}
