#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <map>


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

private:
    Node m_root;
};

#endif // TRIE_H
