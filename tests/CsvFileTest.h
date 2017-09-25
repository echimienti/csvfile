/*
 * csvfileTest.h
 *
 *  Created on: Sep 22, 2017
 *      Author: echimienti
 */
#pragma once

#include <gtest/gtest.h>

class CsvFileTestBase : public ::testing::Test {
public:
    CsvFileTestBase();
    virtual ~CsvFileTestBase();

    void SetUp() override;
    void TearDown() override;

    std::string count_lines_in_testfile();
    void clean_test_files();

};
