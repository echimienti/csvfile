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
#include "common_utils.cpp"


template <class T>
void CsvFile<T>::print_entries() {
    /* Print the csv entries from the 2 dimensional vector
     *
     * @param: no parameters
     * @return: void
     */
    for(int i=0;i<m_row;i++) {
        for(int j=0;j<m_col;j++) {
            cout << m_csv_vector[i][j] << ' ';
        }
        cout << endl;
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
    ifstream inf(m_filename.c_str());

    // If we couldn't open the input file stream for reading
    if (!inf) {
        // Print an error and exit
        cerr << m_filename << " could not be opened for reading!" << endl;
        exit(1);
    }

    // While there's still stuff left to read
    while (!inf.eof()) {
        getline(inf, csvLine, '\n');
        if (csvLine.size() != 0) {
            stringstream csvLineStream(csvLine);
            vector<T> csvItem;
            for(int j=0;j<m_col;j++) {
                getline(csvLineStream, element, ',');
                csvItem.push_back(element);
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
    while (!inf.eof()) {
        getline(inf, csvLine, '\n');
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
            // std::cerr writes (typically error messages) to the standard error stream stderr (unbuffered)
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
                outf << m_csv_vector[i][j] << ",";
            }
            outf << endl;
        }

        outf.close();
    }
    catch (std::exception &ex) {
        std::cout << "Exception: " << ex.what() << "!\n";
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
        for(int j=0;j<m_col;j++) {
            if(m_csv_vector[i][j].find(search_for) != std::string::npos) {
                search_found = m_csv_vector[i][j];
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
string CsvFile<int>::search_entry(string search_for) {
    /* Searches a string from the csv entries from 2 dimensional vector
     *
     * @param: search_for: string to search for
     * @return: void
     */
    string search_found;

    for(int i=0;i<m_row;i++) {
        for(int j=0;j<m_col;j++) {
            if(to_string(m_csv_vector[i][j]).find(search_for) != std::string::npos) {
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
    for(int j=0;j<m_col;j++) {
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
                for(int j=0;j<m_col;j++) {
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
                else if(confirm == "q") {
                    break;
                }

                cout << endl;
            }
        }
        else {
            if(mode == "delete"){
                for(int j=0;j<m_col;j++) {
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
