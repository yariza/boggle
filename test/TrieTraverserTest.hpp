#include "TrieTraverser.h"
#include <gtest/gtest.h>

class TrieTraversalTestFixture : public ::testing::Test
{
protected:
    TrieTraversalTestFixture() {};
    ~TrieTraversalTestFixture() {};

    void SetUp() {
        trie_ = new Trie();
        traverser_ = new TrieTraverser(trie_);
    }

    void TearDown() {
        delete traverser_;
        delete trie_;
    }

    Trie* trie_;
    TrieTraverser* traverser_;
};

TEST_F(TrieTraversalTestFixture, EmptyTrieReturnsNull) {
    ASSERT_TRUE(traverser_->next() == NULL);
}

TEST_F(TrieTraversalTestFixture, TrieReturnsNullAtEnd) {
    trie_->insert("california");
    char* first = traverser_->next();
    char* second = traverser_->next();
    ASSERT_TRUE(second == NULL);
    delete[] first;
}

TEST_F(TrieTraversalTestFixture, TrieTraversalInOrder) {
    trie_->insert("calendar");
    trie_->insert("california");
    char* first = traverser_->next();
    ASSERT_STREQ(first, "calendar");
    char* second = traverser_->next();
    ASSERT_STREQ(second, "california");
    delete[] first;
    delete[] second;
}

TEST_F(TrieTraversalTestFixture, TrieTraversalPrefix) {
    trie_->insert("prefix");
    trie_->insert("pre");
    trie_->insert("prefixes");
    char* first = traverser_->next();
    ASSERT_STREQ(first, "pre");
    char* second = traverser_->next();
    ASSERT_STREQ(second, "prefix");
    char* third = traverser_->next();
    ASSERT_STREQ(third, "prefixes");
    delete[] first;
    delete[] second;
    delete[] third;
}
