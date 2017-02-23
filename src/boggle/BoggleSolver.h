#ifndef __BOGGLE_SOLVER_H__
#define __BOGGLE_SOLVER_H__

#include <vector>
#include <string>
#include <deque>
#include "Trie.h"

namespace boggle {

struct Results
{
    const char* const* Words;    // pointers to unique found words, each terminated by a non-alpha char
    unsigned           Count;    // total number of words found
    unsigned           Score;    // total score for all words found
    void*              UserData; // ignored
};

class BoggleSolver
{
public:
    BoggleSolver();
    // convenience constructor that automatically loads dictionary path
    BoggleSolver(const char* dictionaryPath);
    // will call FreeDictionary if not already called
    ~BoggleSolver();

    // input dictionary is a file with one word per line
    void LoadDictionary(const char* path);

    // load a single word into dictionary. Returns success state
    bool LoadWordInDictionary(std::string& word);
    bool LoadWordInDictionary(const char* word);

    // this func may be called multiple times
    // board: will be exactly width * height chars, and 'q' represents the 'qu' Boggle cube
    Results FindWords(const char* board, unsigned width, unsigned height); // << TODO

    // 'results' is identical to what was returned from FindWords
    void FreeWords(Results results);
    void FreeDictionary();

    // replace occurences of 'qu' with 'q'.
    // if an instance of 'q' exists where it is not followed by
    // 'u', the function returns false.
    bool replaceQuWithQ(std::string& string);
    void replaceQWithQu(std::string& string);

    // retrieve dictionary Trie for debugging/testing
    Trie* dictionary();
private:
    int ScoreForWord(const char* word);
    void TraverseBoard(const char* board, unsigned width, unsigned height,
                         Trie* words, int& totalScore,
                         int x, int y);
    // recursive function.
    void TraverseBoard(const char* board, unsigned width, unsigned height,
                                     Trie* words, int& totalScore,
                                     int x, int y,
                                     Trie* dictionary,
                                     std::deque<char>& currentWord,
                                     bool* visitedBoard);
    Trie* m_dictionary;
};

}

#endif
