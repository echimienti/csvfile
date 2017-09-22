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
    CsvFileTestBase() = default;
    virtual ~CsvFileTestBase() = default;

    void SetUp() override;
    void TearDown() override;

    std::string system_cmd_output();
    void clean_test_files();

};
