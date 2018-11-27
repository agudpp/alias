#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <map>
#include <vector>


struct Node {
    Node(bool aIsLefa = false) :
        isLeaf(aIsLefa)
    {}

    std::map<char, Node> children;
    bool isLeaf;
};

class trie
{
public:
    trie();
    ~trie();

    ///
    /// \brief insert
    /// \param word
    /// \return
    ///
    bool
    insert(const std::string& word);

    ///
    /// \brief search
    /// \param word
    /// \return
    ///
    bool
    search(const std::string& word) const;

    ///
    /// \brief getSuggestions
    /// \param prefix
    /// \param suggests
    /// \param deep
    ///
    void
    getSuggestions(const std::string& prefix, std::vector<std::string>& suggests, int deep = 24);


private:

private:
    Node m_root;
};

#endif // TRIE_H
