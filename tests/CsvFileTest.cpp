/*
 * CsvFileTestBase.cpp
 *
 *  Created on: Sep 22, 2017
 *      Author: echimienti
 */
#include <fstream>

#include "CsvFileTest.h"


using namespace std;


CsvFileTestBase::CsvFileTestBase() = default;
CsvFileTestBase::~CsvFileTestBase() = default;

void CsvFileTestBase::SetUp()
{
    clean_test_files();

}


void CsvFileTestBase::TearDown()
{

}

string CsvFileTestBase::count_lines_in_testfile()
{
    ifstream inf("out.txt");
    string line;
    string act_line;

    while (!inf.eof()) {
        getline(inf, line, '\n');
        if (line.size() != 0) {
            act_line = line;
        }
    }
    inf.close();

    return act_line;
}

void CsvFileTestBase::clean_test_files()
{
    // if test files exist delete them first
    const int test_file_exist = system("ls test.csv > /dev/null 2>&1");
    if(test_file_exist == 0) {
        system("rm test.csv");
    }

    const int out_file_exist = system("ls out.txt > /dev/null 2>&1");
    if(out_file_exist == 0) {
        system("rm out.txt");
    }

    const int test_csv_utf8_file_exist = system("ls test.csv.utf8 > /dev/null 2>&1");
    if(test_csv_utf8_file_exist == 0) {
        system("rm test.csv.utf8");
    }
}
