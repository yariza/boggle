#ifndef __TRIE_TRAVERSER_H__
#define __TRIE_TRAVERSER_H__

#include "Trie.h"
#include <deque>

struct TrieTraverserState
{
    char value;
    Trie* node;
    unsigned childIndex;

    TrieTraverserState(char my_value, Trie* my_node, unsigned my_childIndex)
    : value(my_value)
    , node(my_node)
    , childIndex(my_childIndex)
    {}

    ~TrieTraverserState() {}
};

/**
 * @brief A non-STL-conforming iterator object for traversing
 * Tries and getting the string representation of a given node.
 * @details The Trie traverser goes through all existing objects
 * respresented in the Trie and returns their string representation.
 * As the Trie is traversed in ascending order, output will be sorted
 * alphabetically.
 */
class TrieTraverser
{
public:
    TrieTraverser(Trie* head);
    ~TrieTraverser();

    // goes to the next existing node in the Trie and returns its
    // c-string representation. User is responsible for clearing
    // memory.
    char* next();

private:
    char* buildString();

    Trie* m_head;
    // maintains a call-stack-like representation of traversal state.
    std::deque<TrieTraverserState> m_states;
};
#endif
