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


vector<string> an_entry {"Piet Puk",
                           "Pukstraat 1",
                           "1111 PP",
                           "Pukstad",
                           "011 11111111",
                           "pietje@puk.nl"
};

vector< vector <string> > an_address { an_entry };

vector<int> num_entry {1, 2, 3, 4, 5};

vector< vector <int> > a_num_vec { num_entry };

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
}

TEST(csv_write_read_file_stringTest, csv_write_readPos) {

    clean_test_files();

    CsvFile<string> csv("test.csv", an_address, 6);
    csv.write_file("app");
    vector < vector<string> > csv_vector = csv.get_m_csv_vector();

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<string> csv_r("test.csv", 6);
    csv_r.read_file();

    ASSERT_EQ(csv.count_vector_rows(), 1) << "Size of vector should be 1";

    clean_test_files();
}

TEST(csv_search_file_stringTest, csv_search_Pos) {
    clean_test_files();

    CsvFile<string> csv("test.csv", an_address, 6);
    csv.write_file("in");
    string expected_found = "Piet Puk";
    string search_found = csv.search_entry("Puk");

    ASSERT_EQ(search_found, expected_found) << "Did not find Pietje Puk";

    clean_test_files();
}

TEST(csv_search_file_stringTest, csv_search_Neg) {
    clean_test_files();

    CsvFile<string> csv("test.csv", an_address, 6);
    csv.write_file("in");
    string expected_found = "Pietje Puk";
    string search_found = csv.search_entry("Pietje Puk");

    ASSERT_NE(search_found, expected_found) << "Did not find Pietje Puk";

    clean_test_files();
}

TEST(csv_write_read_file_intTest, csv_write_readPos) {

    clean_test_files();

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("app");
    vector < vector<int> > csv_vector = csv.get_m_csv_vector();

    system("wc -l test.csv|cut -d\" \" -f1 > out.txt"); // execute the linux command

    string act_lines = system_cmd_output();

    ASSERT_EQ(csv.get_m_csv_vector().size(), 1) << "Expected 1 row written/read";
    ASSERT_EQ(act_lines, "1") << "Expected 1 row written/read";

    CsvFile<int> csv_r("test.csv", 5);
    csv_r.read_file();

    ASSERT_EQ(csv.count_vector_rows(), 1) << "Size of vector should be 1";

    clean_test_files();
}

TEST(csv_search_file_intTest, csv_search_Pos) {
    clean_test_files();

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "3";
    string search_found = csv.search_entry("3");

    ASSERT_EQ(search_found, expected_found) << "3";

    clean_test_files();
}

TEST(csv_search_file_intTest, csv_search_Neg) {
    clean_test_files();

    CsvFile<int> csv("test.csv", a_num_vec, 5);
    csv.write_file("in");
    string expected_found = "-1";
    string search_found = csv.search_entry("2");

    ASSERT_NE(search_found, expected_found) << "2";

    clean_test_files();
}
