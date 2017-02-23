#include "BoggleSolver.h"
#include "TrieTraverser.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

#define MIN_WORD_LENGTH 3
const int SCORE_CHART[9] = {
    0, 0, 0, // words length 0-2 don't score
    1, // 3
    1, // 4
    2, // 5
    3, // 6
    5, // 7
    11 // 8+
};
#define SCORE_CHART_SIZE 9

// really hacky macro to facilitate indexing of 1D array as if it
// were 2D
#define GET2D(board, x, y) (board[y*width + x])
#define SET2D(board, x, y, val) (board[y*width + x] = val)

using namespace boggle;

BoggleSolver::BoggleSolver()
{
    m_dictionary = new Trie();
}

BoggleSolver::BoggleSolver(const char* dictionaryPath)
{
    BoggleSolver();
    LoadDictionary(dictionaryPath);
}

BoggleSolver::~BoggleSolver()
{
    if (m_dictionary) {
        FreeDictionary();
    }
}

void BoggleSolver::LoadDictionary(const char* path)
{
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Error opening path " << path << ". Exiting." << std::endl;
        exit(1);
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.back() == '\r') {
            line.erase(line.length() - 1, 1);
        }
        if (LoadWordInDictionary(line)) {
            count++;
        } else {
        }
    }
    std::cout << "Loaded " << count << " words." << std::endl << std::endl;

    file.close();
}

bool BoggleSolver::LoadWordInDictionary(std::string& line)
{
    for (int i=0; i<line.length(); i++) {
        char c = line[i];
        if (!(('a' <= c && c <= 'z') ||
              ('A' <= c && c <= 'Z'))) {
            std::cout << line << " contains illegal character " << c << "! Skipping." << std::endl;
            return false;
        }
    }
    if (!replaceQuWithQ(line)) {
        std::cout << line << " contains q not followed by u! Skipping." << std::endl;
        return false;
    }
    // std::cout << "inserting as " << line << std::endl;
    if (!m_dictionary->insert(line.c_str())) {
        return false;
    }
    return true;
}

bool BoggleSolver::LoadWordInDictionary(const char* word)
{
    std::string string(word);
    return LoadWordInDictionary(string);
}

Results BoggleSolver::FindWords(const char* board, unsigned width, unsigned height)
{
    // store results in a Trie to remove duplicates
    Trie* words = new Trie();
    int score = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TraverseBoard(board, width, height,
                          words, score,
                          x, y);
        }
    }

    Results results;
    char** wordsArray = new char*[words->size()];
    TrieTraverser traverser(words);
    for (unsigned i=0; i<words->size(); i++) {
        char* word = traverser.next();
        wordsArray[i] = word;
    }
    results.Words = wordsArray;
    results.Count = words->size();
    results.Score = score;
    results.UserData = NULL;
    delete words;
    return results;
}

void BoggleSolver::TraverseBoard(const char* board, unsigned width, unsigned height,
                                 Trie* words, int& totalScore,
                                 int x, int y)
{
    std::deque<char> currentWord;
    bool *visitedBoard = new bool[width*height](); // this zeros out memory
    TraverseBoard(board, width, height,
                  words, totalScore,
                  x, y,
                  m_dictionary, currentWord, visitedBoard);
}

void BoggleSolver::TraverseBoard(const char* board, unsigned width, unsigned height,
                                 Trie* words, int& totalScore,
                                 int x, int y,
                                 Trie* dictionary,
                                 std::deque<char>& currentWord,
                                 bool* visitedBoard)
{
    if (x < 0 || width <= x || y < 0 || height <= y) {
        return;
    }
    if (GET2D(visitedBoard, x, y)) {
        // we've already gone here before
        return;
    }
    char c = GET2D(board, x, y);
    Trie* child = dictionary->child(c);
    if (child) {
        currentWord.push_back(c);
        SET2D(visitedBoard, x, y, true);

        // check if this completes a word
        if (child->exists()) {
            // we found a word!
            bool success;
            const char* insertWord;
            std::string word(currentWord.begin(), currentWord.end());
            // std::cout << "found word at " << word << std::endl;
            int length;
            int score;
            replaceQWithQu(word);
            insertWord = word.c_str();

            score = ScoreForWord(insertWord);
            if (score > 0) {
                success = words->insert(insertWord);
                if (success) {
                    totalScore += score;
                }
            }
        }

        TraverseBoard(board, width, height,
                      words, totalScore,
                      x+1, y, child, currentWord, visitedBoard);
        TraverseBoard(board, width, height,
                      words, totalScore,
                      x-1, y, child, currentWord, visitedBoard);

        TraverseBoard(board, width, height,
                      words, totalScore,
                      x, y+1, child, currentWord, visitedBoard);
        TraverseBoard(board, width, height,
                      words, totalScore,
                      x, y-1, child, currentWord, visitedBoard);

        TraverseBoard(board, width, height,
                      words, totalScore,
                      x+1, y+1, child, currentWord, visitedBoard);
        TraverseBoard(board, width, height,
                      words, totalScore,
                      x-1, y-1, child, currentWord, visitedBoard);

        TraverseBoard(board, width, height,
                      words, totalScore,
                      x+1, y-1, child, currentWord, visitedBoard);
        TraverseBoard(board, width, height,
                      words, totalScore,
                      x-1, y+1, child, currentWord, visitedBoard);

        currentWord.pop_back();
        SET2D(visitedBoard, x, y, false);
    }
}

int BoggleSolver::ScoreForWord(const char* word)
{
    int length = strlen(word);
    // retrieve score from length
    int score;
    if (SCORE_CHART_SIZE <= length)
        score = SCORE_CHART[SCORE_CHART_SIZE-1];
    else
        score = SCORE_CHART[length];
    return score;
}

void BoggleSolver::FreeWords(Results results)
{
    for (int i=0; i<results.Count; i++) {
        delete[] results.Words[i];
    }
    delete[] results.Words;
}

void BoggleSolver::FreeDictionary()
{
    delete m_dictionary;
    m_dictionary = NULL;
}

bool BoggleSolver::replaceQuWithQ(std::string& string)
{
    // replace 'qu' with 'q'.
    // unfortunately, not all words that contain 'q' proceed
    // with 'u'. Examples include 'qintar', 'Iraqi', and others.
    // In these cases, the word is ignored.
    std::string original = string;
    bool legalString = true;

    size_t index = string.find('q');
    while (index != std::string::npos) {
        // check if next char is 'u'
        if (index < string.length() - 1 &&
            string[index+1] == 'u') {
            string.replace(index, 2, "q");
            index++;
        } else { // if not, mark as illegal
            return false;
        }

        index = string.find('q', index);
    }
    return true;
}

void BoggleSolver::replaceQWithQu(std::string& string)
{
    size_t index = string.find('q');
    while (index != std::string::npos) {
        string.replace(index, 1, "qu");
        index += 2;
        index = string.find('q', index);
    }
}

Trie* BoggleSolver::dictionary()
{
    return m_dictionary;
}
