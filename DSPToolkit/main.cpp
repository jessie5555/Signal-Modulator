/* Final Project: Jessie Fehrenbach
 * Section: A
 *
 * Digital signal Generator: Converts a message into a digital signal.
 *
 *
 *
 */



#include <iostream>
#include <fstream>
#include <vector>
#include "functions.h"

int main() {
    ofstream fout;
    string word;
    DigitalSignalProcess test;
    getline(cin, word); // asks user for the sentence they would like to modulate

    test.WordsToComplex(word); // converts words to signal for convolution with RRC

    test.RRC();// makes the RRC graph


    test.Modulation();
    cout << "the phrase you ended up modulating is: " << test.GetWord() <<endl;

    int err = test.PrintJson(test.filter ,"signal" );
    if(err){
        cerr << "you've got an error"<<endl;
        return err;
    }
    cout << "Awesome!! now you can view these signals using the python script attached!!!";


    return 0;
}
