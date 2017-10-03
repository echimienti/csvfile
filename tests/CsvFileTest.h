/*
 * csvfileTest.h
 *
 *  Created on: Sep 22, 2017
 *      Author: echimienti
 */
#pragma once

#include <gtest/gtest.h>

#include "csvfile.h"


class CsvFileTestBase : public ::testing::Test {
public:
    CsvFileTestBase() = default;
    virtual ~CsvFileTestBase() = default;

    void SetUp() override;
    void TearDown() override;

    void CsvFileWriteReadTest(std::vector< std::vector <std::string> >, int);
    void CsvFileTestSearch(std::vector< std::vector <std::string> >, std::string, bool, std::string);
    void CsvFileTestModify(std::vector< std::vector <std::string> >, std::string, std::vector<std::string>, std::string);

    std::string count_lines_in_testfile();
    void clean_test_files();

};
