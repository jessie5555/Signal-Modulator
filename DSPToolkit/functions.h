/* Final Project: Jessie Fehrenbach
 * Section: A
 *
 * Digital signal Generator: Converts a message into a digital signal.
 *
 *
 *
 */

#ifndef DSPTOOLKIT_FUNCTIONS_H
#define DSPTOOLKIT_FUNCTIONS_H
#include <vector>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fstream>
#include <complex>
using namespace std;

class DigitalSignalProcess{
private:
    const static int _fftSize = 1024;
    vector <complex <double> > _symbols;
    string _word;
    double data[_fftSize];

public:
    DigitalSignalProcess();
    void WordsToComplex(string words);
    void DFT (complex<double> input[], complex <double> output[], int size); // discrete fourier transform
    void IDFT (complex<double> input[], complex <double> *output, int size); // inverse discrete fourier transform
    void RRC(); // generated RootRaised Cosine impulse
    int PrintJson(complex <double> data[], string file);
    complex <double> filter[_fftSize]; // array public to avoid creating getter with pointers.
    void Modulation();
    void introduction();
    string GetWord();




};

#endif //DSPTOOLKIT_FUNCTIONS_H
