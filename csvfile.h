/*
 * csvfile.h
 *
 *  Created on: Apr 3, 2017
 *  @author: E. Chimienti
 *  @version 0.1 28/03/17
 */

#ifndef CSV_FILE_PROJ_CSVFILE_H_
#define CSV_FILE_PROJ_CSVFILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


using namespace std;

//! gets user input
string get_input(string);

//! copies a file used in backup and restore
void copyFile(string, string, string);

/*! CsvFile Class: a class provides functions for doing operations on CSV files
 * (comma separated files).
 * It reads it in into a 2 dimensional vector of vectors
 */
template <class T>
class CsvFile {
private:
    string m_filename;
    vector< vector<T> > m_csv_vector;
    int m_row;
    int m_col;
public:
    //! Constructor for all operations except adding a row
    CsvFile(string fileName, int col) :
            m_filename(fileName), m_row(count_csv_file_lines()), m_col(col)
    {

    }

    //! Constructor for adding a row
    CsvFile(string fileName, vector< vector<T> > a_csvLine, int col) :
        m_filename(fileName), m_csv_vector(a_csvLine), m_row(count_csv_file_lines()), m_col(col)
    {

    }

    // return m_csv_vector
    //! Function for getting m_csv_vector
    vector< vector<T> > get_m_csv_vector() { return m_csv_vector; }

    //! Count csv lines in csv file
    int count_csv_file_lines();

    //! Reads csv file and pushes to 2 dimensional vector
    void read_file();

    //! Writes 2 dimensional vector to csv file
    void write_file(string);

    //! Count csv lines in 2 dimensional vector
    int count_vector_rows();

    //! Prints entries of 2 dimensional vector
    void print_entries();

    //! Searches for string in 2 dimensional vector
    string search_entry(string);

    //! Deletes row from 2 dimensional vector
    void delete_modify(string, string);

    //! Backs up csv file
    void backup_data();

    //! Restores from backup file
    int restore_data();

    //! Update a field in 2 dimensional vector
    void update_field(int);

    //! Converts UTF16-LE file to UTF-8 file
    int ConvertUTF16();
};

#endif /* CSV_FILE_PROJ_CSVFILE_H_ */
