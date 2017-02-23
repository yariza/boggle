#include <iostream>
#include <time.h>
#include <sys/time.h>
#include "BoggleSolver.h"

using namespace boggle;

int main(int args, char** argv)
{
    BoggleSolver solver;
    solver.LoadDictionary("./words");

    // unsigned width = 6;
    // unsigned height = 6;
    // char board[36] =
    // {
    //     'd', 's', 'r', 'o', 'd', 'g',
    //     't', 'e', 'm', 'e', 'n', 's',
    //     'r', 'a', 's', 'i', 't', 'o',
    //     'd', 'g', 'n', 't', 'r', 'p',
    //     'r', 'e', 'i', 'a', 'e', 's',
    //     't', 's', 'c', 'l', 'p', 'd'
    // };

    unsigned width = 3;
    unsigned height = 3;
    char board[9] =
    {
        'd', 'z', 'x',
        'e', 'a', 'i',
        'q', 'u', 't'
    };
    // unsigned width = 4;
    // unsigned height = 4;
    // char board[16] =
    // {
    //     'd', 'z', 'x', 'y',
    //     'e', 'a', 'i', 'a',
    //     'q', 'u', 't', 'm',
    //     'a', 'p', 'e', 'r'
    // };
    std::cout << "Board:" << std::endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::cout << board[y*width+x];
        }
        std::cout << std::endl;
    }

    // measure time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    Results results = solver.FindWords(board, width, height);

    gettimeofday(&end, NULL);

    std::cout << std::endl << "Result: " << std::endl;
    for (int i=0; i<results.Count; i++) {
        std::cout << results.Words[i] << std::endl;
    }
    std::cout << "Words: " << results.Count << std::endl;
    std::cout << "Score: " << results.Score << std::endl;

    double startTime = ((double)start.tv_sec)*1.0e3 + ((double)start.tv_usec)*1.0e-3;
    double endTime = ((double)end.tv_sec)*1.0e3 + ((double)end.tv_usec)*1.0e-3;
    std::cout << "Time taken: " << (endTime - startTime) << "ms" << std::endl;

    solver.FreeWords(results);
}
