#include "Trie.h"
#include <gtest/gtest.h>
#include <deque>

class TrieTestFixture : public ::testing::Test {
protected:
    TrieTestFixture() {};
    ~TrieTestFixture() {};

    void SetUp() {
        trie_ = new Trie();
    }

    void TearDown() {
        delete trie_;
    }

    Trie *trie_;
};

TEST_F(TrieTestFixture, EmptyTrieNodeDoesNotExist) {
    ASSERT_FALSE(trie_->exists());
}

TEST_F(TrieTestFixture, InsertCharCreatesNode) {
    trie_->insert("c");
    int index = 'c' - 'a';
    Trie** children = trie_->children();
    ASSERT_TRUE(children[index] != NULL);
}

TEST_F(TrieTestFixture, ChildIndexIsCorrect) {
    trie_->insert("c");
    int index = 'c' - 'a';
    Trie** children = trie_->children();
    ASSERT_EQ(children[index], trie_->child('c'));
}

TEST_F(TrieTestFixture, InsertedIndexExists) {
    trie_->insert("c");
    int index = 'c' - 'a';
    Trie** children = trie_->children();
    Trie* node_c = children[index];
    ASSERT_TRUE(node_c->exists());
}

TEST_F(TrieTestFixture, InsertedWordExists) {
    trie_->insert("california");
    trie_->insert("cal");
    ASSERT_TRUE(trie_->exists("california"));
    ASSERT_TRUE(trie_->exists("cal"));
}

TEST_F(TrieTestFixture, UninsertedWordDoesNotExist) {
    ASSERT_FALSE(trie_->exists("cal"));
    trie_->insert("cal");
    ASSERT_FALSE(trie_->exists("florida"));
    ASSERT_FALSE(trie_->exists("california"));
}

TEST_F(TrieTestFixture, TrieIsCaseInsensitive) {
    trie_->insert("California");
    ASSERT_TRUE(trie_->exists("california"));
}

TEST_F(TrieTestFixture, TrieSize) {
    trie_->insert("a");
    trie_->insert("b");
    ASSERT_EQ(trie_->size(), 2);
}

TEST_F(TrieTestFixture, DuplicateInsertionReturnsFalse) {
    trie_->insert("duplicate");
    ASSERT_FALSE(trie_->insert("duplicate"));
}

TEST_F(TrieTestFixture, DuplicateInsertionDoesNotAffectSize) {
    trie_->insert("a");
    trie_->insert("b");
    ASSERT_EQ(trie_->size(), 2);
    trie_->insert("b");
    ASSERT_EQ(trie_->size(), 2);
}

TEST_F(TrieTestFixture, TrieInsertSucceedsForPrefix) {
    bool success1 = trie_->insert("pre");
    ASSERT_TRUE(success1);
    bool success2 = trie_->insert("prefix");
    ASSERT_TRUE(success2);
}
