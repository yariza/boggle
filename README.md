# Boggle solver

Yujin Ariza

Included is a solver for the game Boggle, written in C++.

## Compilation

The project uses CMake in order to build. Compilation steps are below:

    # cd into boggle directory is not already done so
    mkdir build
    cd build
    cmake ..
    make

CMake will then compile all files in the `src/boggle` directory into a library
`libboggle.a`, and will link the `main.cpp` file against it, creating the
`solver` executable. Additionally, CMake will download a copy of Google Test,
and will create a test executable called `btest`, from files in `test`.

All code is tested on Mac OS X Yosemite, using clang 7.0.2.

## Running

To run, simply execute the binary from within the build directory with no parameters:

    ./solver

This will read in the included dictionary `words` (which has been copied into
the build directory as part of the CMake build process), print out a default
board configuration, and will print out the results and timing information.

To run tests, simply run:

    ./btest

This will invoke gtest and run the included tests.

## Approach

A Trie is utilized for both the storage of the dictionary and the result words.
A Trie is a data structure much like a tree; however the children are an
ordered list of N elements, corresponding to the number of characters in the
alphabet, pointing to NULL or the next descendant. With this approach, very
large sets of strings can be reduced to a Trie representation, saving storage
as words with the same prefix share the same nodes. In addition, the ordered nature of the children allows one to traverse the Trie in order, resulting in an alphabetical list of the Trie contents.

Various approaches to the representation of the children list exist, each with
its own time and space tradeoffs. The simplest approach, having a 26-element
array of node pointers, results in a quick lookup time but suffers from poor
memory density if entries do not share similar prefixes. This is the approach
used in this implementation. Other ideas include using a linked list to store
the children, which gets rid of the empty pointers, but is non-constant in the
lookup time complexity of a specific child. Another approach involves reducing
the 26-letter alphabet into 6-letter 'chunks'. Then, a single letter from the
original alphabet can be represented by two of these 6-letter characters, as
6*6 > 26. This dramatically increases memory density, but at the cost of
runtime (2 times slower lookup in the worst-case scenario).

When traversing through the board to search for words, a recursive function is
used to keep track of the word path while retaining state. Of the function
parameters, a deque of chars is used to keep track of the encountered
characters, while being able to retrieve the entire word at any point in the
call stack. Also, a boolean array is persisted between calls to keep track of
locations already visited. It is worth noting that as the dimensions of the
board become larger, the possible length of the call stack becomes much larger
by a factor of width*height; therefore the possibility of hitting the stack
limit of the OS becomes a more pressing issue. To combat this, one can move all
the function parameter data into a stack data structure in heap memory. In my
implementation, however, I simultaneously traverse through the Trie node in the
dictionary at the same time that I traverse through the board, so that I do not
recurse if I see that a prefix corresponding to what was encountered on the
board does not exist in the dictionary; therefore, this puts a hard limit of
the depth of recursion to the depth of my dictionary Trie, which equates to the
length of the longest word (typically, this would be an acceptable number of
calls).

The word list used in this project is taken from Mac OS X's system wordlist, located at `/usr/share/dict/words`. Using the example on the website, the 3x3 board results in a match of 34 words, with a score of 94. On my machine (a 2011 Macbook Pro), this takes 0.280029ms to compute.
