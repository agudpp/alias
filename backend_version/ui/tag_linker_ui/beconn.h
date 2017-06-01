#ifndef BECONN_H
#define BECONN_H

#include <string>
#include <vector>
#include <map>

class BEConn
{
public:
    struct InitData {
        int port;
        std::string addr;
    };

    struct Elem {
        int id;
        std::string content;
    };

    struct QueryResult {
        std::vector<std::string> matched_tags;
        std::vector<std::string> expanded_tags;
        std::map<std::string, std::vector<Elem> > expanded_results;
    };

    struct Query {
        std::vector<std::string> tags;
        std::string q;
    };

public:
    BEConn();

    ///
    /// \brief init
    /// \param id
    /// \return
    ///
    bool
    init(const InitData& id);

    ///
    /// \brief disconnect
    /// \return
    ///
    bool
    disconnect(void);

    ///
    /// \brief processQuery
    /// \param q
    /// \param result
    /// \return
    ///
    bool
    processQuery(const Query& q, QueryResult& result);

private:
    // TODO: the socket thing here
};

#endif // BECONN_H
