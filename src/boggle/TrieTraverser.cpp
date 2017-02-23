#include "TrieTraverser.h"
#include <algorithm>

TrieTraverser::TrieTraverser(Trie* head)
: m_head(head)
, m_states()
{
    TrieTraverserState beginState('\0', head, 0);
    m_states.push_back(beginState);
}

TrieTraverser::~TrieTraverser()
{
    m_states.clear();
}

char* TrieTraverser::next()
{
    while (!m_states.empty()) {
        // get latest state
        TrieTraverserState& state = m_states.back();
        Trie* curNode = state.node;
        unsigned childIndex = state.childIndex;
        if (childIndex >= NUM_ALPHABET) {
            // if child index is out of bounds, pop deque and increment counter
            m_states.pop_back();
            if (!m_states.empty()) {
                m_states.back().childIndex++;
            }
        } else {
            // check node pertaining to child index
            char c = 'a' + childIndex;
            Trie* child = curNode->child(c);
            if (child) {
                // push new state
                TrieTraverserState childState(c, child, 0);
                m_states.push_back(childState);
                // check if exists; if so return String
                if (child->exists()) {
                    return buildString();
                }
            } else {
                state.childIndex++;
            }
        }

    }
    // we've reached the end
    return NULL;
}

char* TrieTraverser::buildString()
{
    char* buffer = new char[m_states.size()];
    unsigned index = 0;
    std::deque<TrieTraverserState>::iterator it;
    for (it = m_states.begin(); it != m_states.end(); it++) {
        TrieTraverserState& state = *it;
        if (state.node != m_head) {
            buffer[index] = state.value;
            index++;
        }
    }
    buffer[index] = '\0';
    return buffer;
}
