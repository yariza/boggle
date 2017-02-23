# Minimum cmake version required
cmake_minimum_required(VERSION 2.8)

project(boggle)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set (CMAKE_CXX_COMPILER g++)

# Add directory with macros
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# Add cmake file with useful functions
include (Common)

# Boggle library
append_files( Headers "h" src/boggle )
append_files( Sources "cpp" src/boggle )

set(BOGGLE_INCLUDE_DIR src/boggle)
set(BOGGLE_LIBRARY_DIR ${CMAKE_BINARY_DIR}/${CMAKE_FIND_LIBRARY_PREFIXESb}boggle.a)
set(BOGGLE_LIBRARY boggle)
add_library(${BOGGLE_LIBRARY} ${Sources} ${Headers})

# Main executable
include_directories(${BOGGLE_INCLUDE_DIR})
add_executable(solver src/main.cpp)
target_link_libraries(solver ${BOGGLE_LIBRARY})

# copy dictionary into binary directory for convenience
copy_files(solver ${PROJECT_SOURCE_DIR}/words ${CMAKE_BINARY_DIR})
copy_files(solver ${PROJECT_SOURCE_DIR}/6x6.txt ${CMAKE_BINARY_DIR})

# Tests
include(gtest)

add_executable(btest test/main.cpp)
target_include_directories(btest PUBLIC ${GTEST_INCLUDE_DIR} test)
target_link_libraries(btest ${GTEST_LIBRARY} ${BOGGLE_LIBRARY})

enable_testing()
add_test(NAME Test COMMAND btest)
