/*
 * common_utils.cpp
 *
 *  Created on: May 3, 2017
 *  @author: E. Chimienti
 *  @version 0.1 03/05/17
 */

#include <iostream>

using namespace std;

string get_input(string a_text) {
    /* Get and return user input. Provides also input validation.
     *
     * @param a_text: Provide the text to question the user about the input
     * @return: Returns the input as a string
     */

    // Loop until user enters a valid input
    while (true) {

        cout << a_text << " ";
        string inp;
        getline(cin, inp);

        // Check for failed extraction
        if (cin.fail()) // has a previous extraction failed?
        {
            // handle failure
            cin.clear(); // put back in 'normal' operation mode
            cin.ignore(32767, '\n'); // and remove the bad input
            cout << "Input is invalid.  Please try again.\n";
        } else {
            // the user can't enter a meaningless string value, don't need to worry about validating
            return inp;
        }
    }
}
