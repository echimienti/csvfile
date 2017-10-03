cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${GTEST_INCLUDE_DIRS}
)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(
    runTests
    main_test.cpp
    ../csvfile.cpp
    ../common_utils.cpp
    ../convert_utf16.cpp

    CsvFileTest.cpp
    csvfile_test.cpp
)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
