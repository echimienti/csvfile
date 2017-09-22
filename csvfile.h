/*
 * csvfile.h
 *
 *  Created on: Apr 3, 2017
 *  @author: E. Chimienti
 *  @version 0.1 28/03/17
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


//! gets user input
std::string get_input(std::string);

//! copies a file used in backup and restore
void copyFile(std::string, std::string, std::string);

/*! CsvFile Class: a class provides functions for doing operations on CSV files
 * (comma separated files).
 * It reads it in into a 2 dimensional vector of vectors
 */
template <class T>
class CsvFile {
private:
    std::string m_filename;
    std::vector< std::vector<T> > m_csv_vector;
    int m_row;
    int m_col;
public:
    //! Constructor for all operations except adding a row
    CsvFile(std::string, int);

    //! Constructor for adding a row
    CsvFile(std::string, std::vector< std::vector<T> >, int);

    // return m_csv_vector
    //! Function for getting m_csv_vector
    std::vector< std::vector<T> > get_m_csv_vector() { return m_csv_vector; }

    //! Count csv lines in csv file
    int count_csv_file_lines();

    //! Reads csv file and pushes to 2 dimensional vector
    void read_file();

    //! Writes 2 dimensional vector to csv file
    void write_file(std::string);

    //! Count csv lines in 2 dimensional vector
    int count_vector_rows();

    //! Prints entries of 2 dimensional vector
    void print_entries();

    //! Searches for std::string in 2 dimensional vector
    std::string search_entry(std::string);

    //! Deletes or modifies a row from 2 dimensional vector
    int delete_modify(std::string, std::string, bool isTest=false, std::vector<std::string> inp={});

    //! Backs up csv file
    int backup_data();

    //! Restores from backup file
    int restore_data();

    //! Update a field in 2 dimensional vector
    void update_field(int, bool isTest=false, std::vector<std::string> inp={});
};
