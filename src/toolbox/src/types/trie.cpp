#include <toolbox/types/trie.h>

#include <queue>


namespace toolbox {


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

////////////////////////////////////////////////////////////////////////////////
void
trie::getSuggestions(const std::string& prefix, std::vector<std::string>& suggests, int deep)
{
    const Node* node = &m_root;
    const int len = prefix.size();

    // go to the proper position
    for (int idx = 0; idx < len; ++idx) {
        auto childIt = node->children.find(prefix[idx]);
        if (childIt == node->children.end()) {
            return;
        }
        node = &(childIt->second);
    }

    // from now we go deep nodes for each branch of the trie
    std::queue<std::pair<const Node*, std::string> > toIt;
    toIt.push(std::make_pair(node, ""));
    // TODO: use here the deep level, if not could be too much.
    //       for now we will use it as max num of results to return
    int maxNumResults = deep;
    while (!toIt.empty() && maxNumResults > 0) {
        auto curr = toIt.front();
        toIt.pop();

        // if it is leaf we have a word here and do nothing
        if (curr.first->isLeaf) {
            suggests.push_back(prefix + curr.second);
            --maxNumResults;
        }
        for (auto& kv : curr.first->children) {
            toIt.push(std::make_pair(&kv.second, curr.second + std::string{kv.first}));
        }
    }
}

// namespace toolbox
