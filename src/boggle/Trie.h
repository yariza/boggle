#ifndef __TRIE_H__
#define __TRIE_H__

#include <iostream>
#include <deque>

#define NUM_ALPHABET 26

/**
 * @brief A Trie of alphabetic characters
 * @details A Trie is a data structure similar to a tree, useful for
 * storing a set of character strings. In each Trie node, a single
 * character represenging the head of the string is stored, and a list
 * of children linking to more nodes, representing the tail of the
 * string.
 */
class Trie {
public:
    Trie();
    ~Trie();

    // all char* must be null-terminated.
    bool insert(const char* word);

    Trie* child(const char c);
    bool exists(const char* word);
    unsigned size();

    // accessor functions for debugging/testing
    bool exists();
    Trie** children();
private:
    unsigned m_size;
    // boolean value for whether word corresopnding to node exists.
    bool m_value;
    // a pointer to a child for each of the 26 characters in alphabet.
    // The downside of this approach is that for nodes lower down in
    // the tree, null pointers will flood the children and lead to a
    // waste of space. Possible ways to combat this include reducing
    // the alphabet to 6 letters so that one can represent a single
    // alphabet using two 6-base characters, dramatically improving
    // storage density while making lookup time twice as slow at worst.
    // Other strategies exist, such as using a Linked List to represent
    // the children instead of an array, but this cuts more heavily
    // into traversal time, as getting the Nth element is a non-constant
    // operation.
    Trie* m_children[NUM_ALPHABET];
};

#endif
