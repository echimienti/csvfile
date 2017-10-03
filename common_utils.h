/*
 * common_utils.h
 *
 *  Created on: Jun 26, 2017
 *      Author: echimienti
 */

#pragma once


bool is_number(const std::string&);
std::string get_input(std::string);
void copyFile(std::string, std::string, std::string);
std::ifstream openFileForRead(std::string);
void sleep(int);
std::string check_byte_order_mark(const char *, size_t);
