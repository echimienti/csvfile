/*
 * common_utils.h
 *
 *  Created on: Jun 26, 2017
 *      Author: echimienti
 */

#ifndef LIBS_CSVFILE_COMMON_UTILS_H_
#define LIBS_CSVFILE_COMMON_UTILS_H_


using namespace std;


const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";
const char *UTF_8_BOM = "\xEF\xBB\xBF";
const char *UTF_32_BE_BOM = "\x00\x00\xFE\xFF";
const char *UTF_32_LE_BOM = "\xFF\xFE\x00\x00";

bool is_number(const string&);
string get_input(string);
void copyFile(string, string, string);
ifstream openFileForRead(string);
void sleep(int);
string check_byte_order_mark(const char *, size_t);

#endif /* LIBS_CSVFILE_COMMON_UTILS_H_ */
