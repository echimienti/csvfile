/*
 * convert_utf16.h
 *
 *  Created on: Jun 21, 2017
 *      Author: echimienti
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "common_utils.h"


/*! UTF16 Class: a class provides functions for doing operations
 * UTF16 files
 */
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

    //! Converts UTF16-LE file to UTF-8 file
    int ConvertUTF16ToUTF8();

    //! Creates UTF16-LE file
    void create_utf16_file();
};
