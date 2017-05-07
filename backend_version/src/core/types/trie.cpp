#include "trie.h"

////////////////////////////////////////////////////////////////////////////////
trie::trie()
{

}

trie::~trie()
{

}

////////////////////////////////////////////////////////////////////////////////
bool
trie::insert(const std::string& word)
{
    Node* node = &m_root;
    const int len = word.size();

    for (int idx = 0; idx < len; ++idx) {
        node = &(node->children[word[idx]]);
    }
    const bool isNewNode = node->isLeaf == false;
    node->isLeaf = true;

    return isNewNode;
}

////////////////////////////////////////////////////////////////////////////////
bool
trie::search(const std::string& word) const
{
    const Node* node = &m_root;
    const int len = word.size();

    for (int idx = 0; idx < len; ++idx) {
        // check if we have a child for this character
        auto childIt = node->children.find(word[idx]);
        if (childIt == node->children.end()) {
            // there are no words with this
            return false;
        }
        // continue it
        node = &(childIt->second);
    }
    return node->isLeaf;
}
