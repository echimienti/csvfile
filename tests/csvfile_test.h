/*
 * csvfile_test.cpp
 *
 *  Created on: May 9, 2017
 *  @author: E. Chimienti
 *  @version 0.1 09/05/17
 *
 */
#include <vector>


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

vector<string> an_entry_var {"\"Pietje, Pukkie\"",
                             "Puk",
                             "Pukstraat 1",
                             "1111 PP",
                             "Pukstad",
                             "011 11111111",
                             "06123456789",
                             "piet@puk.nl"
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

string system_cmd_output() {
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

void clean_test_files() {
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

// string tests
TEST(csv_write_read_file_stringTest, csv_write_readPos) {

    clean_test_files();

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<string> csv_r("test.csv", 6);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";

    clean_test_files();
}

TEST(csv_multipleLine_quotedStringTest, csv_search_Pos) {
    clean_test_files();

    an_address.push_back(an_entry_var);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 2) << "Expected 2 rows written/read";
    ASSERT_EQ(act_lines, "2") << "Expected 2 rows written/read";

    an_address.pop_back();
    clean_test_files();
}

TEST(csv_search_file_stringTest, csv_search_Pos) {
    clean_test_files();

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "Puk";
    string search_found = csv.search_entry("Puk");

    ASSERT_EQ(expected_found, search_found) << "Did not find Puk";

    clean_test_files();
}

TEST(csv_search_file_stringTest, csv_search_Neg) {
    clean_test_files();

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "Pietje";
    string search_found = csv.search_entry("Pietje");

    ASSERT_NE(expected_found, search_found) << "Should not find Pietje";

    clean_test_files();
}

TEST(csv_search_file_quotedStringTest, csv_search_Pos) {
    clean_test_files();

    an_address.push_back(an_entry_var);

    CsvFile<string> csv("test.csv", an_address, 8);
    csv.write_file("in");
    string expected_found = "\"Pietje, Pukkie\"";
    string search_found = csv.search_entry("Pietje");

    ASSERT_EQ(expected_found, search_found) << "Should have found Pietje, Pukkie";

    an_address.pop_back();
    clean_test_files();
}

// integer tests
TEST(csv_write_read_file_intTest, csv_write_readPos) {

    clean_test_files();

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<int> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";

    clean_test_files();
}

TEST(csv_search_file_intTest, csv_search_Pos) {
    clean_test_files();

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "3";
    string search_found = csv.search_entry("3");

    ASSERT_EQ(expected_found, search_found) << "Did not find 3";

    clean_test_files();
}

TEST(csv_search_file_intTest, csv_search_Neg) {
    clean_test_files();

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "-1";
    string search_found = csv.search_entry("-1");

    ASSERT_NE(expected_found, search_found) << "Should not find -1";

    clean_test_files();
}

TEST(csv_add_second_entry_intTest, csv_search_Pos) {
    clean_test_files();

    a_num_vec.push_back(num_entry2);

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    ASSERT_EQ(2, csv.count_vector_rows()) << "Size of vector should be 2";

    csv.write_file("in");
    ASSERT_EQ(2, csv.count_csv_file_lines()) << "Nr of rows in csv file should be 2";

    clean_test_files();
}

// double tests
TEST(csv_write_read_file_doubleTest, csv_write_readPos) {

    clean_test_files();

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("app");

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(1, csv.get_m_csv_vector().size()) << "Expected 1 row written/read";
    ASSERT_EQ("1", act_lines) << "Expected 1 row written/read";

    CsvFile<double> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(1, csv.count_vector_rows()) << "Size of vector should be 1";

    clean_test_files();
}

TEST(csv_search_file_doubleTest, csv_search_Pos) {
    clean_test_files();

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("in");
    cout << "Printing entries:" << endl;
    string expected_found = "3.99999999999996";
    string search_found = csv.search_entry("3.99999999999996");

    ASSERT_EQ(expected_found, search_found) << "3.99999999999996";

    clean_test_files();
}

TEST(csv_search_file_doubleTest, csv_search_Neg) {
    clean_test_files();

    CsvFile<double> csv("test.csv", a_double_vec, 5);
    csv.write_file("in");
    string expected_found = "2.99999999999997";
    string search_found = csv.search_entry("2.99999999999997");

    ASSERT_NE(expected_found, search_found) << "Should not find 2.99999999999997";

    clean_test_files();
}

TEST(csv_byteOrderMarkUTF8, csv_bom_Pos) {
    clean_test_files();

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

    clean_test_files();
}

TEST(csv_byteOrderMarkUTF16, csv_bom_Pos) {
    clean_test_files();

    string csvLine;
    create_utf16_file();

    ifstream inf("test.csv");
    getline(inf, csvLine, '\n');
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    ASSERT_EQ("UTF-16-LE", bom_type) << "Should have found UTF-16-LE";

    clean_test_files();
}

TEST(csv_ConvertUTF16ToUTF8, csv_Pos) {
    clean_test_files();

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

    clean_test_files();
}
