#include <gtest/gtest.h>
#include "TrieTest.hpp"
#include "TrieTraverserTest.hpp"
#include "BoggleSolverTest.hpp"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
