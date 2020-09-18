#ifndef TOOLBOX_TRIE_H_
#define TOOLBOX_TRIE_H_

#include <string>
#include <map>
#include <vector>


namespace toolbox {


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
    getSuggestions(const std::string& prefix, std::vector<std::string>& suggests, int deep = 24) const;


private:

private:
    Node m_root;
};

} // namespace toolbox

#endif // TOOLBOX_TRIE_H_
