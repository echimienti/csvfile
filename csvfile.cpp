/*
 * csvfile.cpp
 *
 *  Created on: Apr 3, 2017
 *  @author: E. Chimienti
 *  @version 0.1 28/03/17
 *
 *  Purpose: 2 dimensional vector. Reading from and writing to csv file
 */
#include <stdexcept>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cassert>

#include "csvfile.h"
#include "convert_utf16.cpp"
#include "common_utils.cpp"


using namespace std;


template <class T>
//! Constructor for all operations except adding a row
CsvFile<T>::CsvFile(string fileName, int col) :
            m_filename(fileName), m_row(count_csv_file_lines()), m_col(col)
{

}

template <class T>
//! Constructor for adding a row
CsvFile<T>::CsvFile(string fileName, vector< vector<T> > a_csvLine, int col) :
        m_filename(fileName), m_csv_vector(a_csvLine), m_row(count_csv_file_lines()), m_col(col)
{

}

template <class T>
void CsvFile<T>::print_entries() {
    /* Print the csv entries from the 2 dimensional vector
     * with the line number ahead of it
     *
     * @param: no parameters
     * @return: void
     */
    string input_lines;
    string scrolling;
    int nr_lines_per_time;
    int nr_lines_printed = 0;

    // Ask user how many lines per time to print when more than 20 rows
    if(m_row > 20) {
        while(!is_number(input_lines)){
            input_lines = get_input("How many lines per time to print, enter a nr: ");
        }
    }
    else {
        input_lines = to_string(m_row);
    }

    nr_lines_per_time = stoi(input_lines);

    // printout the rows with elements
    while(nr_lines_printed < m_row) {
        for(int i=nr_lines_printed;i < nr_lines_printed + nr_lines_per_time;i++) {
            // add line number in front of each line
            cout << i + 1 << " ";
            // print each element
            for(uint j=0;j<m_csv_vector[i].size();j++) {
                cout << setprecision (15) << m_csv_vector[i][j] << " ";
            }
            cout << endl;
        }

        nr_lines_printed += nr_lines_per_time;

        // handle when last amount of lines is left is less than nr_lines_per_time
        if(nr_lines_printed + nr_lines_per_time >= m_row){
            nr_lines_per_time = m_row - nr_lines_printed;
        }

        // wait for user before continue printing
        if(m_row > 20) {
            scrolling = get_input("Press any key for next lines or q to quit");
        }

        if(scrolling == "q"){
            break;
        }
    }
}

template <class T>
int CsvFile<T>::count_csv_file_lines(){
    /* Count the csv entries from the csv file
     *
     * @param: no parameters
     * @return: lines_count: int nr of lines found
     */
    ifstream inf(m_filename.c_str());
    int lines_count = 0;
    string line;

    while (getline(inf, line))
            ++lines_count;

    return lines_count;
}

template <class T>
void CsvFile<T>::read_file() {
    /* Reads in the csv entries from the csv file and pushes them into a
     * 2 dimensional vector of vectors
     *
     * @param: no parameters
     * @return: void
     */
    string csvLine;
    string element;

    ifstream inf = openFileForRead(m_filename);

    //check if need to convert from utf-16
    getline(inf, csvLine, '\n');
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    // after reading first line set file back to beginning
    inf.clear();
    inf.seekg(0, ios::beg);

    if(bom_type == "UTF-16-LE"){
        UTF16 utf16(m_filename.c_str(), (m_filename+".utf8").c_str());
        utf16.ConvertUTF16ToUTF8();
    }

    // For each line extract the elements
    while (getline(inf, csvLine, '\n')) {
        if (csvLine.size() != 0) {
            int nr_el_comma = std::count(csvLine.begin(), csvLine.end(), ',');
            string str_buf = "";
            vector<T> csvItem;
            stringstream csvLineStream(csvLine);

            for(int j=0;j<=nr_el_comma;j++) {
                getline(csvLineStream, element, ',');

                // check element starts with double quote and element is not only double quote
                size_t n_dquotes = std::count(element.begin(), element.end(), '"');
                if(n_dquotes > 0 && element.size() > 0) {
                    // if two consecutive elements contain double quotes
                    // handle "xxxx,xxxx",
                    if(str_buf.size() > 0){
                        str_buf = str_buf + "," + element;
                        element = str_buf;
                        str_buf = "";
                    }
                    // if last element in row has quotes
                    else if(j==nr_el_comma){
                        str_buf = "";
                    }
                    else{
                        str_buf+=element;
                    }
                }
                else {
                    // if next elements do not contain double quotes
                    // handle "xxxx,xxxx,,,,
                    if(str_buf.size() > 0){
                        element = str_buf + "," + element;
                    }
                    str_buf = "";
                }

                // if buffer is empty push the element
                if(str_buf.size() == 0) {
                    csvItem.push_back(element);
                }
            }
            m_csv_vector.push_back(csvItem);
        }
    }
    inf.close();
}

template <>
void CsvFile<int>::read_file() {
    /* Reads in the csv entries from the csv file and pushes them into a
     * 2 dimensional vector of vectors
     *
     * @param: no parameters
     * @return: void
     */
    string csvLine;
    string element;

    ifstream inf = openFileForRead(m_filename);

    // While there's still stuff left to read
    while (getline(inf, csvLine, '\n')) {
        if (csvLine.size() != 0) {
            stringstream csvLineStream(csvLine);
            vector<int> csvItem;
            for(int j=0;j<m_col;j++) {
                getline(csvLineStream, element, ',');
                csvItem.push_back(stoi(element));
            }
            m_csv_vector.push_back(csvItem);
        }
    }
    inf.close();
}

template <>
void CsvFile<double>::read_file() {
    /* Reads in the csv entries from the csv file and pushes them into a
     * 2 dimensional vector of vectors
     *
     * @param: no parameters
     * @return: void
     */
    string csvLine;
    string element;

    ifstream inf = openFileForRead(m_filename);

    // While there's still stuff left to read
    while (getline(inf, csvLine, '\n')) {
        if (csvLine.size() != 0) {
            stringstream csvLineStream(csvLine);
            vector<double> csvItem;
            for(int j=0;j<m_col;j++) {
                getline(csvLineStream, element, ',');
                csvItem.push_back(stod(element));
            }
            m_csv_vector.push_back(csvItem);
        }
    }
    inf.close();
}

template <class T>
void CsvFile<T>::write_file(string mode) {
    /* Writes 2 dimensional vector to a csv file.
     *
     * @param: no parameters
     * @return: void
     */
    try {
        ofstream outf;

        if(mode == "app") {
            cout << "Opening file in append mode for writing" << endl;
            outf.open(m_filename.c_str(), ios_base::app);
        }
        else {
            cout << "Opening file in out mode for writing" << endl;
            outf.open(m_filename.c_str(), ios::out);
        }

        if( !outf || !outf.is_open() )
        {
            // outf should be open
            throw ios_base::failure("*** error: could not open output file\n");
        }

        m_row = m_csv_vector.size();

        for(int i=0;i<m_row;i++) {
            int col = m_csv_vector[i].size();
            for(int j=0;j<col;j++) {
                if(j < col - 1){
                    outf << m_csv_vector[i][j] << ",";
                }
                else {
                    outf << m_csv_vector[i][j];
                }
            }
            outf << endl;
        }

        outf.close();
    }
    catch (const std::ios_base::failure& e) {
        cout << "Exception: " << e.what() << "!\n";
    }
}

template <class T>
int CsvFile<T>::count_vector_rows() {
    /* Count the csv entries from 2 dimensional vector
     *
     * @param: no parameters
     * @return: lines_count: int nr of lines found
     */
    return m_csv_vector.size();
}

template <class T>
string CsvFile<T>::search_entry(string search_for) {
    /* Searches a string from the csv entries from 2 dimensional vector
     *
     * @param: search_for: string to search for
     * @return: void
     */
    string search_found;

    for(int i=0;i<m_row;i++) {
        for(uint j=0;j<m_csv_vector[i].size();j++) {
            if(m_csv_vector[i][j].find(search_for) != string::npos) {
                search_found = m_csv_vector[i][j];
                for(uint x=0; x<m_csv_vector[i].size(); x++){
                    cout <<  m_csv_vector[i][x] << " ";
                }
                cout << endl;
                break; // At first element found stop searching the elements
            }
        }
    }

    return search_found;
}

template <>
string CsvFile<int>::search_entry(string search_for) {
    /* Searches an integer from the csv entries from 2 dimensional vector
     *
     * @param: search_for: string to search for
     * @return: void
     */
    string search_found;

    for(int i=0;i<m_row;i++) {
        for(int j=0;j<m_col;j++) {
            if(to_string(m_csv_vector[i][j]).find(search_for) != string::npos) {
                search_found = to_string(m_csv_vector[i][j]);
                for(int x=0; x<m_col; x++){
                    cout <<  m_csv_vector[i][x] << " ";
                }
                cout << endl;
                break; // At first element found stop searching the elements
            }
        }
    }

    return search_found;
}

template <>
string CsvFile<double>::search_entry(string search_for) {
    /* Searches a double from the csv entries from 2 dimensional vector
     *
     * @param: search_for: string to search for
     * @return: void
     */
    string search_found;
    stringstream strstr;

    for(int i=0;i<m_row;i++) {
        for(int j=0;j<m_col;j++) {
            strstr << fixed << setprecision( 15 ) << m_csv_vector[i][j];
            string str = strstr.str();

            if(str.find(search_for) != string::npos) {
                //remove trailing 0
                str.erase ( str.find_last_not_of('0') + 1, string::npos );
                search_found = str;
                for(int x=0; x<m_col; x++){
                    cout << m_csv_vector[i][x] << " ";
                }
                cout << endl;
                break; // At first element found stop searching the elements
            }
            else {
                strstr.str(string()); // clear stringstream
            }
        }
    }

    return search_found;
}

template <class T>
int CsvFile<T>::backup_data() {
    /* Creates backup directory if not exists and makes a backup of csv file
     *
     * @param: no parameters
     * @return: void
     */
    const int check_out = system("ls backup > /dev/null 2>&1");

    // if backup dir does not exist create it first
    if(check_out != 0) {
        try {
            const int dir_err = system("mkdir backup");

            if(dir_err != 0) {
                throw runtime_error("Error creating directory!");
                return -1;
            }
        }
        catch (const runtime_error& e) {
            cout << "Exception: " << e.what() << "!\n";
        }
    }

    // backup m_filename
    copyFile(m_filename, "backup/" + m_filename + ".bkup", "backup of");
    return 0;
}

template <class T>
int CsvFile<T>::restore_data() {
    /* Restores backup of the csv file
     *
     * @param: no parameters
     * @return: void
     */
    string confirm = get_input("Do you want to restore from backup?\n"
                               "Confirm y/n or quit: q:");

    if(confirm == "y") {
        const int check_err = system("ls backup > /dev/null 2>&1");

        // if backup dir does not exist create it first
        if(check_err != 0) {
            cout << "No backup directory, so no backups yet" << endl;
            return -1;
        }

        // restore backup to m_filename
        copyFile("backup/" + m_filename + ".bkup", m_filename, "restore");
        return 0;
    }
    else if (confirm == "n" or confirm == "q"){
        return 0;
    }
    else {
        restore_data();
    }
    return 0;
}

template <class T>
void CsvFile<T>::update_field(int row, bool isTest, vector<string> inp) {
    /* Updates a column in a row of 2 dimensional array
     *
     * @param: row: the row in which a column can be updated
     * @return: void
     */
    string confirm;
    string change_string = "";

    for(uint j=0;j<m_csv_vector[row].size();j++) {
        cout << m_csv_vector[row][j] << endl;

        if(isTest && j == 0){
            confirm = inp[3];
            if(confirm == "y"){
                change_string = inp[4];
            }
        }
        else if(isTest && j > 0){
            if(change_string != ""){
                confirm = inp[5];
                confirm = inp[6];
            }
            else {
                confirm = inp[4];
            }
        }
        else{
            if(!isTest){
                confirm = get_input("\nDo you want to modify this string?\n"
                                           "Confirm y/n or quit: q:");
            }
        }

        if(confirm == "y") {
            if(!isTest){
                change_string = get_input("Enter new string?");
            }
            m_csv_vector[row][j] = change_string;

            cout << "changed to: " << m_csv_vector[row][j] << endl;
        }
        else if (confirm == "n") {
            continue;
        }
        else if(confirm == "q") {
            break;
        }
    }
}

template <>
void CsvFile<int>::update_field(int row, bool isTest, vector<string> inp) {
    /* Updates a column in a row of 2 dimensional array
     *
     * @param: row: the row in which a column can be updated
     * @return: void
     */
    string confirm;
    string change_string;

    for(int j=0;j<m_col;j++) {
        cout << m_csv_vector[row][j] << endl;

        if(isTest && j == 0){
            confirm = inp[3];
            change_string = inp[4];
        }
        else if(isTest && j > 0){
            confirm = inp[5];
            confirm = inp[6];
        }
        else{
            if(!isTest){
                confirm = get_input("Do you want to modify this string?\n"
                                           "Confirm y/n or quit: q:");
            }
        }

        if(confirm == "y") {
            if(!isTest){
                change_string = get_input("Enter new string?");
            }
            m_csv_vector[row][j] = stoi(change_string);

            cout << "changed to: " << m_csv_vector[row][j] << endl;
        }
        else if (confirm == "n") {
            continue;
        }
        else if(confirm == "q") {
            break;
        }
    }
}

template <>
void CsvFile<double>::update_field(int row, bool isTest, vector<string> inp) {
    /* Updates a column in a row of 2 dimensional array
     *
     * @param: row: the row in which a column can be updated
     * @return: void
     */
    string confirm;
    string change_string;

    for(int j=0;j<m_col;j++) {
        cout << m_csv_vector[row][j] << endl;

        if(isTest && j == 0){
            confirm = inp[3];
            change_string = inp[4];
        }
        else if(isTest && j > 0){
            confirm = inp[5];
            confirm = inp[6];
        }
        else{
            if(!isTest){
                confirm = get_input("Do you want to modify this string?\n"
                                    "Confirm y/n or quit: q:");
            }
        }

        if(confirm == "y") {
            if(!isTest){
                change_string = get_input("Enter new string?");
            }
            m_csv_vector[row][j] = stod(change_string);

            cout << "changed to: " << m_csv_vector[row][j] << endl;
        }
        else if (confirm == "n") {
            continue;
        }
        else if(confirm == "q") {
            break;
        }
    }
}

template <class T>
int CsvFile<T>::delete_modify(string mode, string csvSubject, bool isTest, vector<string> inp) {
    /* Deletes or modifies a row in the 2 dimensional array
     *
     * @param: mode: delete or modify
     * @param: csvSubject: the subject of the csv file ( e.g. addresses )
     * @return: int
     */
    string confirm_main;
    int line_nr;
    string confirm_l_all;
    string confirm_line;

    try {
        if(mode != "delete" && mode != "modify"){
            throw logic_error("Error invalid mode: " + mode);
        }
    }
    catch(logic_error const& err){
        cout << err.what() << endl;
        return -1;
    }

    if(isTest){
        assert(inp.size() > 0);
        confirm_main = inp[0];

        if(inp.size() > 1){
            line_nr = stoi(inp[1]);
            if(inp.size() > 2){
                confirm_l_all = inp[2];
                confirm_line = inp[2];
            }
        }
    }
    else {
        confirm_main = get_input("Do you want to " + mode + " " + csvSubject +
                                   " ?\nConfirm " + mode + " y/n:");
    }

    if (confirm_main == "y") {
        if(!isTest){
            line_nr = stoi(get_input("Line number to " + mode + "\nEnter -1 to step through them all:"));
        }
        if(line_nr == -1) {
            for(int i=0;i<m_row;i++) {
                for(uint j=0;j<m_csv_vector[i].size();j++) {
                    cout << m_csv_vector[i][j] << ' ';
                }

                if(!isTest){
                    confirm_l_all = get_input("\nDo you want to " + mode + " this line?\n"
                                                 "Confirm " + mode + ": y/n/ or quit: q:");
                }

                if(confirm_l_all == "y") {
                    cout << mode + " line: " << i << endl;
                    if(mode == "delete"){
                        m_csv_vector.erase(m_csv_vector.begin() + i);
                        i--;
                        m_row--;
                    }
                    else if (mode == "modify"){
                        update_field(i, isTest, inp);
                    }
                }
                if(confirm_l_all == "n") {
                    continue;
                }
                if(confirm_l_all == "q") {
                    break;
                }

                cout << endl;
            }
        }
        else {
            for(uint j=0;j<m_csv_vector[line_nr - 1].size();j++) {
                cout << m_csv_vector[line_nr - 1][j] << ' ';
            }

            if(!isTest){
                confirm_line = get_input("\nDo you want to " + mode + " line: " + to_string(line_nr) + " ?\n"
                                           "Confirm " + mode + " y/n or q to quit:");
            }

            if(confirm_line == "y") {
                if(mode == "delete"){
                    m_csv_vector.erase(m_csv_vector.begin() + (line_nr - 1));
                }
                else if (mode == "modify"){
                    update_field(line_nr - 1, isTest, inp);
                }
            }
            else if(confirm_line == "n" or confirm_line == "q") {
                return 1;
            }
        }
    }
    else if(confirm_main == "n"){
        return 1;
    }
    else{
        delete_modify(mode, csvSubject, isTest, inp);
    }
    return 0;
}

template class CsvFile<string>;
template class CsvFile<int>;
template class CsvFile<double>;

