/*
 * convert_utf16.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: echimienti
 */
#include <iostream>

#include "convert_utf16.h"


using namespace std;

UTF16::UTF16(string utf16_fileName, string utf8_fileName) :
        m_utf16_filename(utf16_fileName), m_utf8_filename(utf8_fileName)
    {

    }

int UTF16::ConvertUTF16ToUTF8(){
    /* Converts an UTF16-LE file to UTF-8
     *
     * @param: no parameters
     * @return: int
     */

    string line_utf16;
    string line_utf8;
    ofstream outf;
    ifstream inf(m_utf16_filename.c_str());

    int line_nr = 1;

    try {
        // If we couldn't open the input file stream for reading
        if (!inf || !inf.is_open()) {
            // Throw an error and
            throw ios_base::failure("*** " + m_utf16_filename + " could not be opened for reading!\n");
        }

        cout << "Opening file in out mode for converting" << endl;
        outf.open(m_utf8_filename.c_str(), ios::binary);

        if (!outf || !outf.is_open()) {
            // Print an error and exit
            throw ios_base::failure("*** " + m_utf8_filename + " could not be opened for converting!\n");
        }
    }
    catch (const std::ios_base::failure& e) {
        cout << "Exception: " << e.what() << "!\n";
    }

    // prepare UTF-8 byte order mark
    line_utf8+=m_byte_order_mark[0];
    line_utf8+=m_byte_order_mark[1];
    line_utf8+=m_byte_order_mark[2];

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
                    // remove remaining upside question marks
                    if(line_utf16[i] == 0x13){
                        line_utf16[i] = '-';
                    }

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

    // rename new utf8 file to the m_utf16_filename
    copyFile(m_utf8_filename.c_str(), m_utf16_filename.c_str(), "move utf8 file to use");
    sleep(500);

    return 0;
}

void UTF16::create_utf16_file(){
    /* create an utf-16-le file with 2 csv entries
     * containing 2 rows with 8 times 11111111
     *
     * @param: no parameters
     * @return: void
     */
    ofstream outf;
    string utf16_filename = m_utf16_filename.c_str();

    try {
        outf.open(utf16_filename, ios::binary);

        if (!outf || !outf.is_open()) {
            // Throw an error
            throw ios_base::failure("*** " + utf16_filename + " could not be opened for converting!\n");
        }
    }
    catch (const std::ios_base::failure& e) {
        cout << "Exception: " << e.what() << "!\n";
    }

    unsigned char byte_order_mark[2];
    byte_order_mark[0] = 0xFF;
    byte_order_mark[1] = 0xFE;
    unsigned char null_byte = 0x00;

    outf << byte_order_mark[0];
    outf << byte_order_mark[1];

    // write 2 utf16 lines
    for(int row=0; row<2; row++){
        for(int e=0; e<8; e++){
            for(int x=0; x<8; x++){
                outf << '1';
                outf << null_byte;
                if(x == 7 && e != 7){
                    outf << ",";
                    outf << null_byte;
                }
            }
        }

        outf << "\r";
        outf << null_byte;
        outf << "\n";
        outf << null_byte;
    }

    outf.close();
}
