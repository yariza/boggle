#include "Trie.h"
#include <cstddef>
#include <cctype>
#include <cstdlib>
#include <iostream>

Trie::Trie()
: m_value(false)
, m_size(0)
{
    for (int i=0; i<NUM_ALPHABET; i++) {
        m_children[i] = NULL;
    }
}

Trie::~Trie()
{
    m_value = false;
    // we deallocate all the children if any are non-null
    for (int i=0; i<NUM_ALPHABET; i++) {
        if (m_children[i]) {
            delete m_children[i];
            m_children[i] = NULL;
        }
    }
}

bool Trie::insert(const char* word)
{
    char c = tolower(*word);
    if (c == '\0') {
        if (m_value) {
            return false;
        }
        m_value = true;
        return true;
    }
    else if ('a' <= c && c <= 'z') {
        int index = c - 'a';
        if (!m_children[index])
            m_children[index] = new Trie();
        // insert next character
        bool success = m_children[index]->insert(word+1);
        if (success) {
            m_size++;
        }
        return success;
    } else {
        std::cerr << "character " << c << " is illegal! Exiting." << std::endl;
        return false;
    }
}

Trie* Trie::child(const char c)
{
    if ('a' <= c && c <= 'z') {
        int index = c - 'a';
        return m_children[index];
    } else {
        std::cerr << "character " << c << " is illegal! Exiting." << std::endl;
        return NULL;
    }
}

bool Trie::exists(const char* word)
{
    char c = tolower(*word);
    if (c == '\0')
        return m_value;
    if ('a' <= c && c <= 'z') {
        int index = c - 'a';
        if (!m_children[index])
            return false;
        return m_children[index]->exists(word+1);
    } else {
        std::cerr << "character " << c << " is illegal! Exiting." << std::endl;
        exit(1);
        return false;
    }
}

bool Trie::exists()
{
    return m_value;
}

unsigned Trie::size()
{
    return m_size;
}

Trie** Trie::children()
{
    return m_children;
}
