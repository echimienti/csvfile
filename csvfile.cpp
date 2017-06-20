/*
 * csvfile.cpp
 *
 *  Created on: Apr 3, 2017
 *  @author: E. Chimienti
 *  @version 0.1 28/03/17
 *
 *  Purpose: 2 dimensional vector. Reading from and writing to csv file
 */

#include "csvfile.h"
#include <stdexcept>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include "common_utils.cpp"
#include <string.h>


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

    if(m_row > 20) {
        while(!is_number(input_lines)){
            input_lines = get_input("How many lines per time to print, enter a nr: ");
        }
    }
    else {
        input_lines = to_string(m_row);
    }

    nr_lines_per_time = stoi(input_lines);

    while(nr_lines_printed < m_row) {
        for(int i=nr_lines_printed;i < nr_lines_printed + nr_lines_per_time;i++) {
            cout << i + 1 << " ";
            for(uint j=0;j<m_csv_vector[i].size();j++) {
                cout << setprecision (15) << m_csv_vector[i][j] << ' ';
            }
            cout << endl;
        }

        nr_lines_printed += nr_lines_per_time;

        if(nr_lines_printed + nr_lines_per_time >= m_row){
            nr_lines_per_time = m_row - nr_lines_printed;
        }

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

const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";
const char *UTF_8_BOM = "\xEF\xBB\xBF";
const char *UTF_32_BE_BOM = "\x00\x00\xFE\xFF";
const char *UTF_32_LE_BOM = "\xFF\xFE\x00\x00";

string check_byte_order_mark(const char *data, size_t size){
    if (size >= 3) {
        if (memcmp(data, UTF_8_BOM, 3) == 0)
            return "UTF-8";
    }
    if (size >= 4) {
        if (memcmp(data, UTF_32_LE_BOM, 4) == 0)
            return "UTF-32-LE";
        if (memcmp(data, UTF_32_BE_BOM, 4) == 0)
            return "UTF-32-BE";
    }
    if (size >= 2) {
        if (memcmp(data, UTF_16_LE_BOM, 2) == 0)
            return "UTF-16-LE";
        if (memcmp(data, UTF_16_BE_BOM, 2) == 0)
            return "UTF-16-BE";
    }
    return "Could not determine";
}

template<class T>
int CsvFile<T>::ConvertUTF16(){

    string line_utf16;
    string line_utf8;
    unsigned char byte_order_mark[3] {0xEF, 0xBB, 0xBF};
    int line_nr = 1;
    ifstream inf(m_filename.c_str());

    // If we couldn't open the input file stream for reading
    if (!inf) {
        // Print an error and exit
        cerr << m_filename << " could not be opened for reading!" << endl;
        exit(1);
    }

    ofstream outf;

    cout << "Opening file in out mode for converting" << endl;
    outf.open((m_filename+".utf8").c_str(), ios::binary);

    if (!outf) {
        // Print an error and exit
        cerr << "m_csv_vector.csv could not be opened for converting!" << endl;
        exit(1);
    }

    // prepare UTF-8 byte order mark
    line_utf8+=byte_order_mark[0];
    line_utf8+=byte_order_mark[1];
    line_utf8+=byte_order_mark[2];

    while( getline(inf, line_utf16, '\n') ){
        uint line_size = line_utf16.size();

        for (uint i=0; i < line_size; i++) {
            /* Start from first char, UTF-16-LE has 2 bytes BOM
             * UTF-8 BOM has 3 bytes
             * take first of 2 bytes of utf16 and leave 2 bytes \r\00
             * at end of line
             */
            if(line_nr == 1){
                if(i > 1 && i%2 == 0 && i < line_size - 2){
                    // save first char after byte 0 and 1 which is UTF-16 BOM
                    // and write it to the 4th position as UTF-8 BOM uses byte 0 till 2
                    if(i == 3){
                        line_utf8+=line_utf16[2];
                    }

                    line_utf8+=line_utf16[i];
                }
            }
            else{
                // take second of 2 bytes and leave 3 bytes \00 at end of line
                if(i%2 == 1 && i < line_size - 3){
                    line_utf8+=line_utf16[i];
                }
            }
        }

        // no new line at end of file
        if(!inf.eof()){
            outf << line_utf8 << endl;
        }

        //reset line_utf8 for next line
        line_utf8 = "";
        line_nr++;
    }

    inf.close();
    outf.close();

    // rename new utf8 file to the m_filename
    copyFile((m_filename+".utf8").c_str(), m_filename.c_str(), "move utf8 file to use");
    sleep(500);

    return 0;
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
    ifstream inf(m_filename.c_str());

    // If we couldn't open the input file stream for reading
    if (!inf) {
        // Print an error and exit
        cerr << m_filename << " could not be opened for reading!" << endl;
        exit(1);
    }

    //check if need to convert from utf-16
    getline(inf, csvLine, '\n');
    string bom_type = check_byte_order_mark(csvLine.c_str(), csvLine.size());

    // after reading first line set file back to beginning
    inf.clear();
    inf.seekg(0, ios::beg);

    if(bom_type == "UTF-16-LE"){
        ConvertUTF16();
    }

    // While there's still stuff left to read
    while (getline(inf, csvLine, '\n')) {
        if (csvLine.size() != 0) {
            int nr_el_comma = count(csvLine.begin(), csvLine.end(), ',');
            string str_buf = "";
            vector<T> csvItem;
            stringstream csvLineStream(csvLine);

            for(int j=0;j<=nr_el_comma;j++) {
                getline(csvLineStream, element, ',');

                // check element starts with double quote and element is not only double quote
                if(element.find("\"") == 0 && element.size() > 1) {
                    str_buf = element;
                } else {
                    str_buf = "";
                }

                if(str_buf.size() > 0) {
                    element = str_buf + element;
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
    ifstream inf(m_filename.c_str());

    // If we couldn't open the input file stream for reading
    if (!inf) {
        // Print an error and exit
        cerr << m_filename << " could not be opened for reading!" << endl;
        exit(1);
    }

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
    ifstream inf(m_filename.c_str());

    // If we couldn't open the input file stream for reading
    if (!inf) {
        // Print an error and exit
        cerr << m_filename << " could not be opened for reading!" << endl;
        exit(1);
    }

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

        if( !outf.is_open() )
        {
            // cerr writes (typically error messages) to the standard error stream stderr (unbuffered)
            // https://en.wikipedia.org/wiki/Stderr
            cout << "*** error: could not open output file\n" ;
        }

        if (!outf) {
            // Print an error and exit
            cerr << "m_csv_vector.csv could not be opened for writing!" << endl;
            exit(1);
        }

        m_row = m_csv_vector.size();

        for(int i=0;i<m_row;i++) {
            for(int j=0;j<m_col;j++) {
                if(j < m_col - 1){
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
    catch (exception &ex) {
        cout << "Exception: " << ex.what() << "!\n";
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
void CsvFile<T>::backup_data() {
    /* Creates backup directory if not exists and makes a backup of csv file
     *
     * @param: no parameters
     * @return: void
     */
    const int check_err = system("ls backup > /dev/null 2>&1");

    // if backup dir does not exist create it first
    if(check_err != 0) {
        const int dir_err = system("mkdir backup");

        if(-1 == dir_err) {
            cout << "Error creating directory!" << endl;
            exit(1);
        }
    }

    // backup m_filename
    copyFile(m_filename, "backup/" + m_filename + ".bkup", "backup of");
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
void CsvFile<T>::update_field(int row) {
    /* Updates a column in a row of 2 dimensional array
     *
     * @param: row: the row in which a column can be updated
     * @return: void
     */
    for(uint j=0;j<m_csv_vector[row].size();j++) {
        cout << m_csv_vector[row][j] << endl;
        string confirm = get_input("Do you want to modify this string?\n"
                                   "Confirm y/n or quit: q:");

        if(confirm == "y") {
            string change_string = get_input("Enter new string?");
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
void CsvFile<int>::update_field(int row) {
    /* Updates a column in a row of 2 dimensional array
     *
     * @param: row: the row in which a column can be updated
     * @return: void
     */
    for(int j=0;j<m_col;j++) {
        cout << m_csv_vector[row][j] << endl;
        string confirm = get_input("Do you want to modify this string?\n"
                                   "Confirm y/n or quit: q:");

        if(confirm == "y") {
            string change_string = get_input("Enter new string?");
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
void CsvFile<double>::update_field(int row) {
    /* Updates a column in a row of 2 dimensional array
     *
     * @param: row: the row in which a column can be updated
     * @return: void
     */
    for(int j=0;j<m_col;j++) {
        cout << m_csv_vector[row][j] << endl;
        string confirm = get_input("Do you want to modify this string?\n"
                                   "Confirm y/n or quit: q:");

        if(confirm == "y") {
            string change_string = get_input("Enter new string?");
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
void CsvFile<T>::delete_modify(string mode, string csvSubject) {
    /* Deletes or modifies a row in the 2 dimensional array
     *
     * @param: mode: delete or modify
     * @param: csvSubject: the subject of the csv file ( e.g. addresses )
     * @return: void
     */
    string confirm = get_input("Do you want to " + mode + " " + csvSubject +
                               " ?\nConfirm " + mode + " y/n:");

    if (confirm == "y") {
        int line_nr = stoi(get_input("Line number to " + mode + "\nEnter -1 to step through them all:"));
        if(line_nr == -1) {
            for(int i=0;i<m_row;i++) {
                for(uint j=0;j<m_csv_vector[i].size();j++) {
                    cout << m_csv_vector[i][j] << ' ';
                }

                string confirm_l = get_input("Do you want to " + mode + " this line?\n"
                                             "Confirm " + mode + ": y/n/ or quit: q:");

                if(confirm_l == "y") {
                    cout << mode + " line: " << i << endl;
                    if(mode == "delete"){
                        m_csv_vector.erase(m_csv_vector.begin() + i);
                        i--;
                        m_row--;
                    }
                    else if (mode == "modify"){
                        update_field(i);
                    }
                    else {
                        throw("Error invalid mode!");
                    }
                }
                if(confirm_l == "q") {
                    break;
                }

                cout << endl;
            }
        }
        else {
            if(mode == "delete"){
                for(uint j=0;j<m_csv_vector[line_nr - 1].size();j++) {
                    cout << m_csv_vector[line_nr - 1][j] << ' ';
                }
                string confirm = get_input("Do you want to " + mode + " line: " + to_string(line_nr) + " ?\n"
                                           "Confirm " + mode + " y/n or q to quit:");

                if(confirm == "y") {
                    m_csv_vector.erase(m_csv_vector.begin() + (line_nr - 1));
                }
                else if(confirm == "n" or confirm == "q") {
                    return;
                }
            }
            else if (mode == "modify"){
                update_field(line_nr - 1);
            }
        }
    }
}

template class CsvFile<string>;
template class CsvFile<int>;
template class CsvFile<double>;

