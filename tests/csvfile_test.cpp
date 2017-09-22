/*
 * csvfile_test.cpp
 *
 *  Created on: May 9, 2017
 *  @author: E. Chimienti
 *  @version 0.1 09/05/17
 *
 */
#include <vector>

#include "csvfile.cpp"
#include "CsvFileTest.h"


using namespace std;


vector<string> an_entry {"Piet",
                         "Puk",
                         "Pukstraat 1",
                         "1111 PP",
                         "Pukstad",
                         "011 11111111",
                         "06123456789",
                         "pietje@puk.nl"
};

vector<string> element_with_comma_entry {"\"Pietje, Pukkie\"",
                             "Puk",
                             "Pukstraat 1",
                             "1111 PP",
                             "Pukstad",
                             "011 11111111",
                             "06123456789",
                             "piet@puk.nl"
};

vector<string> single_element_with_quotes_entry {"\"Pietje, Pukkie",
                             "Puk",
                             "Pukstraat 1",
                             "1111 PP",
                             "Pukstad",
                             "011 11111111",
                             "06123456789",
                             "piet@puk.nl"
};

vector<string> consequtive_elements_with_quotes_entry {"Pietje\",\"Pukkie",
                             "Puk",
                             "Pukstraat 1",
                             "1111 PP",
                             "Pukstad",
                             "011 11111111",
                             "06123456789",
                             "piet@puk.nl"
};

vector<string> last_element_with_quotes_entry {"Pietje",
                             "Puk",
                             "Pukstraat 1",
                             "1111 PP",
                             "Pukstad",
                             "011 11111111",
                             "06123456789",
                             "\"piet@puk.nl"
};

vector< vector <string> > an_address { an_entry };

vector<int> num_entry1 {1, 2, 3, 4, 5};
vector<int> num_entry2 {6, 7, 8, 9, 10};

vector< vector <int> > a_num_vec { num_entry1 };

vector<double> double_entry1 {1.99999999999996,
                              2.99999999999996,
                              3.99999999999996,
                              4.99999999999996,
                              5.99999999999996};

vector< vector <double> > a_double_vec { double_entry1 };

// string tests
TEST_F(CsvFileTestBase, csv_write_read_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<string> csv_r("test.csv", 6);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";
}

TEST_F(CsvFileTestBase, csv_element_quotes_with_comma_test_pos) {

    an_address.push_back(element_with_comma_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 2) << "Expected 2 rows written/read";
    ASSERT_EQ(act_lines, "2") << "Expected 2 rows written/read";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_single_element_with_quotes_test_pos) {

    an_address.push_back(single_element_with_quotes_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 2) << "Expected 2 rows written/read";
    ASSERT_EQ(act_lines, "2") << "Expected 2 rows written/read";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_two_consequtive_elements_with_quotes_test_pos) {

    an_address.push_back(consequtive_elements_with_quotes_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 2) << "Expected 2 rows written/read";
    ASSERT_EQ(act_lines, "2") << "Expected 2 rows written/read";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_last_element_with_quotes_test_pos) {

    an_address.push_back(last_element_with_quotes_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 2) << "Expected 2 rows written/read";
    ASSERT_EQ(act_lines, "2") << "Expected 2 rows written/read";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_search_file_string_test_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "Puk";
    string search_found = csv.search_entry("Puk");

    ASSERT_EQ(expected_found, search_found) << "Did not find Puk";
}

TEST_F(CsvFileTestBase, csv_search_file_string_test_neg) {


    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "Pietje";
    string search_found = csv.search_entry("Pietje");

    ASSERT_NE(expected_found, search_found) << "Should not find Pietje";
}

TEST_F(CsvFileTestBase, csv_search_file_element_quotes_with_comma_test_pos) {

    an_address.push_back(element_with_comma_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "\"Pietje, Pukkie\"";
    string search_found = csv.search_entry("Pietje");

    ASSERT_EQ(expected_found, search_found) << "Should have found Pietje, Pukkie";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_search_single_element_with_quotes_test_pos) {

    an_address.push_back(single_element_with_quotes_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "\"Pietje, Pukkie";
    string search_found = csv.search_entry("Pietje");

    ASSERT_EQ(expected_found, search_found) << "Should have found \"Pietje, Pukkie";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_search_two_consequtive_elements_with_quotes_test_pos) {

    an_address.push_back(consequtive_elements_with_quotes_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "Pietje\",\"Pukkie";
    string search_found = csv.search_entry("Pietje");

    ASSERT_EQ(expected_found, search_found) << "Should have found Pietje\",\"Pukkie";

    an_address.pop_back();
}

TEST_F(CsvFileTestBase, csv_search_last_element_with_quotes_test_pos) {

    an_address.push_back(last_element_with_quotes_entry);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "\"piet@puk.nl";
    string search_found = csv.search_entry("puk.nl");

    ASSERT_EQ(expected_found, search_found) << "Should have found \"piet@puk.nl";

    an_address.pop_back();
}

// modify test
TEST_F(CsvFileTestBase, csv_modify_test_modify_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);

    // Do you want to modify addresses: y
    // Line number to modify\nEnter -1 to step through them all: 1
    // Do you want to modify line: 1?\n Confirm modify y/n or q to quit: y
    // Do you want to modify this string?\n Confirm y/n or quit: q: y
    // Enter new string? Pieter
    // Do you want to modify this string? n
    // Do you want to modify this string? q
    vector<string> input {"y","1","y","y","Pieter","n","q"};
    bool isTest = true;

    int result = csv.delete_modify("modify", "addresses", isTest, input);
    ASSERT_EQ(0, result) << "Failed to modify";

    csv.write_file("in");
    string expected_found = "Pieter";
    string search_found = csv.search_entry("Pieter");

    ASSERT_EQ(expected_found, search_found) << "Should have found Pieter";
}

TEST_F(CsvFileTestBase, csv_modify_test_confirm_main_no_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);

    // Do you want to modify addresses: n
    vector<string> input {"n"};
    bool isTest = true;

    int result = csv.delete_modify("modify", "addresses", isTest, input);
    ASSERT_EQ(1, result) << "Failed to enter no in main confirm";
}

TEST_F(CsvFileTestBase, csv_modify_test_confirm_modify_no_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);

    // Do you want to modify addresses: y
    // Line number to modify\nEnter -1 to step through them all: 1
    // Do you want to modify line: 1?\n Confirm modify y/n or q to quit: n
    vector<string> input {"y","1","n"};
    bool isTest = true;

    int result = csv.delete_modify("modify", "addresses", isTest, input);
    cout << endl;
    ASSERT_EQ(1, result) << "Failed to enter no in modify confirm";
}

TEST_F(CsvFileTestBase, csv_modify_test_confirm_update_quit_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);

    // Do you want to modify addresses: y
    // Line number to modify\nEnter -1 to step through them all: 1
    // Do you want to modify line: 1?\n Confirm modify y/n or q to quit: y
    // Do you want to modify this string?\n Confirm y/n or quit: q: q
    vector<string> input {"y","1","y", "q"};
    bool isTest = true;

    int result = csv.delete_modify("modify", "addresses", isTest, input);
    cout << endl;
    ASSERT_EQ(0, result) << "Failed to enter no in update confirm";
}

TEST_F(CsvFileTestBase, csv_modify_test_confirm_update_no_pos) {

    CsvFile<string> csv("test.csv", an_address, 8);

    // Do you want to modify addresses: y
    // Line number to modify\nEnter -1 to step through them all: 1
    // Do you want to modify line: 1?\n Confirm modify y/n or q to quit: y
    // Do you want to modify this string?\n Confirm y/n or quit: q: n
    // Do you want to modify this string?\n Confirm y/n or quit: q: q
    vector<string> input {"y","1","y", "n", "q"};
    bool isTest = true;

    int result = csv.delete_modify("modify", "addresses", isTest, input);
    cout << endl;
    ASSERT_EQ(0, result) << "Failed to enter no in update confirm";
}

// integer tests
TEST_F(CsvFileTestBase, csv_write_read_file_int_test_pos) {

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<int> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";
}

TEST_F(CsvFileTestBase, csv_search_file_int_test_pos) {

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "3";
    string search_found = csv.search_entry("3");

    ASSERT_EQ(expected_found, search_found) << "Did not find 3";
}

TEST_F(CsvFileTestBase, csv_search_file_int_test_neg) {

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "-1";
    string search_found = csv.search_entry("-1");

    ASSERT_NE(expected_found, search_found) << "Should not find -1";
}

TEST_F(CsvFileTestBase, csv_add_second_entry_int_test_pos) {

    a_num_vec.push_back(num_entry2);

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    ASSERT_EQ(2, csv.count_vector_rows()) << "Size of vector should be 2";

    csv.write_file("in");
    ASSERT_EQ(2, csv.count_csv_file_lines()) << "Nr of rows in csv file should be 2";
}

// double tests
TEST_F(CsvFileTestBase, csv_write_read_file_double_test_pos) {

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(1, csv.get_m_csv_vector().size()) << "Expected 1 row written/read";
    ASSERT_EQ("1", act_lines) << "Expected 1 row written/read";

    CsvFile<double> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";
}

TEST_F(CsvFileTestBase, csv_search_file_double_test_pos) {

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("in");
    cout << "Printing entries:" << endl;
    string expected_found = "3.99999999999996";
    string search_found = csv.search_entry("3.99999999999996");

    ASSERT_EQ(expected_found, search_found) << "3.99999999999996";


}

TEST_F(CsvFileTestBase, csv_search_file_double_test_neg) {

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("in");
    string expected_found = "2.99999999999997";
    string search_found = csv.search_entry("2.99999999999997");

    ASSERT_NE(expected_found, search_found) << "Should not find 2.99999999999997";
}

// byte order mark tests
TEST_F(CsvFileTestBase, csv_byte_order_mark_UTF8_pos) {

    string csvLine;
    ofstream outf;
    outf.open("test.csv", ios::binary);
    unsigned char byte_order_mark[3];
    byte_order_mark[0] = 0xEF;
    byte_order_mark[1] = 0xBB;
    byte_order_mark[2] = 0xBF;
    outf << byte_order_mark[0];
    outf << byte_order_mark[1];
    outf << byte_order_mark[2];
    outf.close();

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("app");

    ifstream inf("test.csv");
    getline(inf, csvLine, '\n');
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-8", bom_type) << "Should have found UTF-8";
}

TEST_F(CsvFileTestBase, csv_byte_order_mark_UTF16_pos) {

    string csvLine;
    create_utf16_file();

    ifstream inf("test.csv");
    getline(inf, csvLine, '\n');
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-16-LE", bom_type) << "Should have found UTF-16-LE";
}

// convert utf16 test
TEST_F(CsvFileTestBase, csv_convert_UTF16_to_UTF8_pos) {

    string csvLine;
    create_utf16_file();
    CsvFile<string> csv("test.csv", 8);

    ifstream inf("test.csv");
    getline(inf, csvLine, '\n');
    inf.close();
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-16-LE", bom_type) << "Should have found UTF-16-LE";

    UTF16 utf16("test.csv", "test.csv.utf8");
    utf16.ConvertUTF16ToUTF8();

    ifstream inf2("test.csv");
    getline(inf2, csvLine, '\n');
    inf2.close();
    string bom_type2 = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-8", bom_type2) << "After converting should have found UTF-8";

    csv.read_file();
    for(uint x=0;x<csv.get_m_csv_vector().size(); x++){
        for(uint y=0; y<csv.get_m_csv_vector()[x].size(); y++){
            if(x==0 && y ==0){
                ASSERT_EQ("\xEF\xBB\xBF" "11111111", csv.get_m_csv_vector()[x][y]) << "Should have ones!";
            }
            else{
                ASSERT_EQ("11111111", csv.get_m_csv_vector()[x][y]) << "Should have ones!";
            }
        }
    }
}
