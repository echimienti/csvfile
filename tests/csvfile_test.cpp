/*
 * csvfile_test.cpp
 *
 *  Created on: May 9, 2017
 *  @author: E. Chimienti
 *  @version 0.1 09/05/17
 *
 */
#include <vector>

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
vector< vector <string> > two_row_addr_with_comma { an_entry, element_with_comma_entry };
vector< vector <string> > two_row_addr_single_elem_quotes { an_entry,
                                                            single_element_with_quotes_entry };
vector< vector <string> > two_conseq_elem_with_quotes { an_entry, consequtive_elements_with_quotes_entry };
vector< vector <string> > last_elem_with_quotes_entry { an_entry, last_element_with_quotes_entry };


vector<int> num_entry1 {1, 2, 3, 4, 5};
vector<int> num_entry2 {6, 7, 8, 9, 10};

vector< vector <int> > a_num_vec { num_entry1 };

vector<double> double_entry1 {1.99999999999996,
                              2.99999999999996,
                              3.99999999999996,
                              4.99999999999996,
                              5.99999999999996};

vector< vector <double> > a_double_vec { double_entry1 };

struct CsvFileParams {
    vector< vector <string> > string_address;
    int expectedLines;
    string test_name;
};

class ParameterizedCsvFileTest : public CsvFileTestBase, public ::testing::WithParamInterface<CsvFileParams> {
public:
    ParameterizedCsvFileTest() :
        CsvFileTestBase() {}
};

// string tests
TEST_P(ParameterizedCsvFileTest, write_read_different_elements) {

    CsvFileWriteReadTest(GetParam().string_address, GetParam().expectedLines);
}


INSTANTIATE_TEST_CASE_P(CsvFile,
    ParameterizedCsvFileTest,
    testing::Values(
        CsvFileParams { an_address, 1, "write_read_pos"},
        CsvFileParams{ two_row_addr_with_comma, 2, "element_quotes_with_comma_pos"},
        CsvFileParams{ two_row_addr_single_elem_quotes, 2, "single_element_with_quotes_pos"},
        CsvFileParams{ two_conseq_elem_with_quotes, 2, "two_consequtive_elements_with_quotes_pos"},
        CsvFileParams{ last_elem_with_quotes_entry, 2, "last_element_with_quotes_pos"}//,
        //CsvFileParams{ a_num_vec, 1, "write_read_file_int_pos"}
    )
);

struct CsvSearchParams {
    vector< vector <string> > test_address;
    string test_name;
    string search_string;
    bool negative_test;
    string expected_found;
};

class ParameterizedCsvSearchTest : public CsvFileTestBase, public ::testing::WithParamInterface<CsvSearchParams> {
public:
    ParameterizedCsvSearchTest() :
        CsvFileTestBase() {}
};

// string search tests
TEST_P(ParameterizedCsvSearchTest, search_string) {
    CsvFileTestSearch(GetParam().test_address, GetParam().search_string, GetParam().negative_test,
                      GetParam().expected_found);
}

INSTANTIATE_TEST_CASE_P(CsvFile,
    ParameterizedCsvSearchTest,
    testing::Values(
        CsvSearchParams { an_address, "search_file_string_pos", "Puk", false, ""},
        CsvSearchParams { an_address, "search_file_string_neg", "Pietje", true, "" },
        CsvSearchParams{ two_row_addr_with_comma, "search_file_element_quotes_with_comma_pos",
                         "Pietje", false, "\"Pietje, Pukkie\""},
        CsvSearchParams{ two_row_addr_single_elem_quotes, "search_single_element_with_quotes_pos",
                         "Pietje", false, "\"Pietje, Pukkie"},
        CsvSearchParams{ two_conseq_elem_with_quotes, "search_two_consequtive_elements_with_quotes_pos",
                         "Pietje", false, "Pietje\",\"Pukkie"},
        CsvSearchParams{ last_elem_with_quotes_entry, "search_last_element_with_quotes_pos",
                         "puk.nl", false, "\"piet@puk.nl"}
    )
);

struct CsvModifyParams {
    vector< vector <string> > test_address;
    string test_name;
    string search_string;
    vector<string> input;
    string expected_found;
};

class ParameterizedCsvModifyTest : public CsvFileTestBase, public ::testing::WithParamInterface<CsvModifyParams> {
public:
    ParameterizedCsvModifyTest() :
        CsvFileTestBase() {}
};

// modify tests
TEST_P(ParameterizedCsvModifyTest, search_string) {
    CsvFileTestModify(GetParam().test_address, GetParam().search_string, GetParam().input,
                      GetParam().expected_found);
}

INSTANTIATE_TEST_CASE_P(CsvFile,
    ParameterizedCsvModifyTest,
    testing::Values(
        CsvModifyParams { an_address, "modify_test_modify_pos", "Pieter", {"y","1","y","y","Pieter","n","q"}, "Pieter"},
        CsvModifyParams { an_address, "modify_confirm_main_no_pos", "", {"n"}, "1"},
        CsvModifyParams { an_address, "modify_confirm_modify_no_pos", "", {"y","1","n"}, "1"},
        CsvModifyParams { an_address, "modify_confirm_update_quit_pos", "", {"y","1","y", "q"}, "0"},
        CsvModifyParams { an_address, "modify_confirm_update_no_pos", "", {"y","1","y", "n", "q"}, "0"}
    )
);


// integer tests
TEST_F(CsvFileTestBase, write_read_file_int_pos) {

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = count_lines_in_testfile();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<int> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";
}

TEST_F(CsvFileTestBase, search_file_int_pos) {

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "3";
    string search_found = csv.search_entry("3");

    ASSERT_EQ(expected_found, search_found) << "Did not find 3";
}

TEST_F(CsvFileTestBase, search_file_int_neg) {

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "-1";
    string search_found = csv.search_entry("-1");

    ASSERT_NE(expected_found, search_found) << "Should not find -1";
}

TEST_F(CsvFileTestBase, add_second_entry_int_pos) {

    a_num_vec.push_back(num_entry2);

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    ASSERT_EQ(2, csv.count_vector_rows()) << "Size of vector should be 2";

    csv.write_file("in");
    ASSERT_EQ(2, csv.count_csv_file_lines()) << "Nr of rows in csv file should be 2";
}

// double tests
TEST_F(CsvFileTestBase, write_read_file_double_pos) {

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = count_lines_in_testfile();

    ASSERT_EQ(1, csv.get_m_csv_vector().size()) << "Expected 1 row written/read";
    ASSERT_EQ("1", act_lines) << "Expected 1 row written/read";

    CsvFile<double> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";
}

TEST_F(CsvFileTestBase, search_file_double_pos) {

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("in");
    cout << "Printing entries:" << endl;
    string expected_found = "3.99999999999996";
    string search_found = csv.search_entry("3.99999999999996");

    ASSERT_EQ(expected_found, search_found) << "3.99999999999996";


}

TEST_F(CsvFileTestBase, search_file_double_neg) {

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("in");
    string expected_found = "2.99999999999997";
    string search_found = csv.search_entry("2.99999999999997");

    ASSERT_NE(expected_found, search_found) << "Should not find 2.99999999999997";
}

// byte order mark tests
TEST_F(CsvFileTestBase, byte_order_mark_UTF8_pos) {

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

TEST_F(CsvFileTestBase, byte_order_mark_UTF16_pos) {

    string csvLine;
    UTF16 utf16("test.csv", "test.csv.utf8");
    utf16.create_utf16_file();

    ifstream inf("test.csv");
    getline(inf, csvLine, '\n');
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-16-LE", bom_type) << "Should have found UTF-16-LE";
}

// convert utf16 test
TEST_F(CsvFileTestBase, convert_UTF16_to_UTF8_pos) {
    string csvLine;
    UTF16 utf16("test.csv.utf16", "test.csv.utf8");
    utf16.create_utf16_file();
    CsvFile<string> csv("test.csv.utf16", 8);

    ifstream inf("test.csv.utf16");
    getline(inf, csvLine, '\n');
    inf.close();
    string utf16_bom = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-16-LE", utf16_bom) << "Should have found UTF-16-LE";

    UTF16 utf8("test.csv.utf16", "test.csv.utf8");
    utf8.ConvertUTF16ToUTF8();

    ifstream inf2("test.csv.utf8");
    getline(inf2, csvLine, '\n');
    inf2.close();
    string utf8_bom = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-8", utf8_bom) << "After converting should have found UTF-8";

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
