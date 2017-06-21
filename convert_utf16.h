/*
 * convert_utf16.h
 *
 *  Created on: Jun 21, 2017
 *      Author: echimienti
 */

#ifndef LIBS_CSVFILE_CONVERT_UTF16_H_
#define LIBS_CSVFILE_CONVERT_UTF16_H_

#include <iostream>
#include <fstream>
#include <string>
#include "common_utils.cpp"


class UTF16 {
private:
    string m_utf16_filename;
    string m_utf8_filename;
    unsigned char m_byte_order_mark[3] {0xEF, 0xBB, 0xBF};
public:
    UTF16(string utf16_fileName, string utf8_fileName) :
        m_utf16_filename(utf16_fileName), m_utf8_filename(utf8_fileName)
    {

    }

    int ConvertUTF16();

    void create_utf16_file();
};

#endif /* LIBS_CSVFILE_CONVERT_UTF16_H_ */
