/*
 * CsvFileTestBase.cpp
 *
 *  Created on: Sep 22, 2017
 *      Author: echimienti
 */
#include <fstream>

#include "CsvFileTest.h"


using namespace std;


void CsvFileTestBase::SetUp()
{
    clean_test_files();

}


void CsvFileTestBase::TearDown()
{
    clean_test_files();
}

void CsvFileTestBase::CsvFileWriteReadTest(vector< vector <string> > test_address, int expectedLines)
{
    CsvFile<string> csv("test.csv", test_address, 8);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = count_lines_in_testfile();

    ASSERT_EQ(expectedLines, csv.get_m_csv_vector().size()) << "Expected " +
            to_string(expectedLines) + " row written/read";
    ASSERT_EQ(act_lines, to_string(expectedLines)) << "Expected " +
            to_string(expectedLines) + " row written/read";

    CsvFile<string> csv_r("test.csv", 8);
    csv_r.read_file();

    ASSERT_EQ(csv.count_vector_rows(), expectedLines) << "Size of vector should be " + to_string(expectedLines);
}

void CsvFileTestBase::CsvFileTestSearch(vector< vector <string> > test_address, string searchString,
                                        bool negativeTest, string expectedFound)
{
    CsvFile<string> csv("test.csv", test_address, 8);
    csv.write_file("in");
    string search_found = csv.search_entry(searchString);

    if(negativeTest)
    {
        ASSERT_NE(searchString, search_found) << "Should not find " + searchString;

    }
    else if(expectedFound != "")
    {
        ASSERT_EQ(expectedFound, search_found) << "Did not find " + searchString;
    }
    else
    {
        ASSERT_EQ(searchString, search_found) << "Did not find " + searchString;
    }
}

void CsvFileTestBase::CsvFileTestModify(vector< vector <string> > test_address, string searchString,
        vector<string> input, string expectedFound)
{
    CsvFile<string> csv("test.csv", test_address, 8);

    // Do you want to modify addresses: y
    // Line number to modify\nEnter -1 to step through them all: 1
    // Do you want to modify line: 1?\n Confirm modify y/n or q to quit: y
    // Do you want to modify this string?\n Confirm y/n or quit: q: y
    // Enter new string? Pieter
    // Do you want to modify this string? n
    // Do you want to modify this string? q
    bool isTest = true;
    int result = csv.delete_modify("modify", "addresses", isTest, input);

    csv.write_file("in");
    string search_found = csv.search_entry(searchString);

    if(input.size() > 5){
        ASSERT_EQ(0, result) << "Failed to modify";
        ASSERT_EQ(expectedFound, search_found) << "Should have found " + searchString;
    }
    else
    {
        ASSERT_EQ(stoi(expectedFound), result) << "Failed to enter no in main confirm";
    }

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

    const int test_csv_utf16_file_exist = system("ls test.csv.utf16 > /dev/null 2>&1");
        if(test_csv_utf16_file_exist == 0) {
            system("rm test.csv.utf16");
        }
}
