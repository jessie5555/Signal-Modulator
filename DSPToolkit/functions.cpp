/* Final Project: Jessie Fehrenbach
 * Section: A
 *
 * Digital signal Generator: Converts a message into a digital signal.
 *
 *
 *
 */

#include "functions.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <stdio.h>
using namespace std;
DigitalSignalProcess:: DigitalSignalProcess(){ // default signal constructor creating signal that has "Hello Signals" inside
    introduction();

}


void DigitalSignalProcess :: introduction() {
    cout << "Welcome to the Digital signal processing toolkit!"<< endl;
    cout << "in this program we'll take an input, convert it to symbols" << endl;
    cout << " by mapping them into the complex plane. Basically were converting the 0's to -1's " << endl;
    cout << "To start well have you enter a string of less than 32 characters" << endl;

}


void DigitalSignalProcess:: WordsToComplex(string words) { // Has an embedded Setter funtion
    //_symbols.resize(words.size()* 8 * sizeof(char)); // resized our symbols vector to the size of our words in binary
    _word = words; // sets _words to word
    vector <complex <double> > tempBinary;
    vector <complex <double> > wordInBinaryBack;
    vector <complex <double> > wordsInBinary;

    complex <double> symbolArr[_fftSize] ={0};
    if(words.size() > 32){
        cerr << "word exceeds maximum modulation size, limiting character" << endl;
        words = words.substr(0,32); // cuts off part of the substring so it can be pushed through
        _word = words;
    }


    for(int i = 0; i < words.size(); i++){
        while ((int)words.at(i) != 0) {// might need to use std :: range thing
            if (((int) words.at(i) % 2) == 0){
                wordInBinaryBack.push_back(-1.0);// has refese bit to map in requency domain
            }
            if (((int) words.at(i) % 2) == 1){ // regular bit mapped in frequency
                wordInBinaryBack.push_back(1.0);
            }// converts the characters to int and turns those integers into backwards binary
            words.at(i) = (int)words.at(i)/2;
            wordInBinaryBack.push_back(0.0);// alternates with zeros to duplicate in frequency domain. so we can tell symbols apart when recieving
        }

    }

    for(int i = wordInBinaryBack.size()-1; i >=0 ; i--){
        _symbols.push_back(wordInBinaryBack.at(i)); // pushes back symbols right side up
    }
    for (int i = wordInBinaryBack.size(); i < _fftSize; i++){// adding zero padding so when convolution wraps there is no interference with noise
        _symbols.push_back(0);
    }

    for(int i =0; i < _fftSize; i++){
        symbolArr[i] = _symbols.at(i);
    }
    PrintJson(symbolArr, "symbol");

}
void DigitalSignalProcess:: DFT (complex<double> input[], complex <double> output[], int size) {
    complex <double> sum = 0;

    const complex <double> J(0,1);
    for (int bin = 0; bin < size; bin++ ){ // increments the bins
        sum = 0; // is part of the sum of the transform
        for (int time = 0; time < size; time++ ){
            sum += input[time] * exp((-2.0 * J * M_PI * (double)time * (double)bin)/ (double)size); // does the transfromation to frequency domain
        }
        output[bin] = sum;
    }


}
void DigitalSignalProcess :: IDFT (complex<double> input [], complex <double> *output, int size){
    complex <double> sum = 0;

    const complex <double> J(0,1);

    for (int bin = 0; bin < size; bin++ ){
        sum = 0;
        for (int freq = 0; freq < size; freq++ ){
            sum +=input[freq] * exp((2.0 * J * M_PI * (double)freq * (double)bin)/ (double)size);
        }
        output[bin] = sum;
    }

}
int DigitalSignalProcess:: PrintJson(complex <double> data[], string file){ // creates Json file and embeds data
    ofstream fout;
    fout.open(file + ".json"); // creates file stream with a json tag
    if (!fout){
        cerr << "Couldn't open file"<< endl; // notifies user if there was an error
        return -1;
    }


    fout << "{\"real\":[";
    for ( int i = 0 ; i < 1024; i++){ // prints all the real values to a json file
        fout << data[i].real();
        if(i < 1024-1){
            fout << ",";
        }
    }
    fout << "],\"imag\":[";
    for ( int i = 0 ; i < 1024; i++){ // prints all the imaginary values right after the real
        fout << data[i].imag();
        if(i < 1024-1){
            fout << ",";
        }
    }
    fout << "]}";
    fout.close(); // closes the file

    fout.open("files.txt", ios_base::app); // opens a file to save all the file names
    if (!fout){
        cerr << "Couldn't open file"<< endl;
        return -1; // checks to see if it errors
    }
    fout << file << " ";
    fout.close();


    return 0;
}


void DigitalSignalProcess :: Modulation() {
    complex <double> arr[_fftSize];
    complex <double> filt[_fftSize];
    complex <double> symbolArr[_fftSize]; // initializes new arrays since weve previously had vectors
    complex <double> finalSig[_fftSize];

    for (int i = 0; i < _fftSize; i++){
        symbolArr[i] = _symbols.at(i); // saves the new values stored in vector symbols into symbolArr
    }
    DFT( symbolArr, arr, _fftSize);
    DFT(filter, filt, _fftSize); // does descrete fourier transforms on our symbols and filter so convolution is easier
    for (int i = 0; i < _fftSize; i++){
        arr[i]= arr[i]*filt[i]; // convolving the values together
    }
    IDFT(arr, finalSig, _fftSize); // takes the new array we just convolved and transfers it into the time domain
    PrintJson(finalSig, "finalSignal"); // saves our data to a json file for python to read

}


void DigitalSignalProcess :: RRC(){ // code for RRC provided by Matthew Smith, Digital signal processing engineer at MITRE
    int N = _fftSize;               // creates a Root Raised Cosine graph that has complex maths that I would be unable to do at the moment
    double PI = acos(-1);
    const complex <double> J(0,1);
    int taps = N/2 + 1;
    const double beta = .35 / 2.0;
    double over = 2.0;
    double dt = 1.0 / over;
    double gain = 1.0 - beta + 4.0 * beta / PI;
    double t = 0;
    double sinarg;
    double cosarg;
    double tmp_tap;
    int ii, cnt;

    double filt[N];
    for (cnt = 0; cnt < N; cnt++) {
        filt[cnt] = 0;   // makes all the values 0
    }


    for (ii = ((taps + 1) / 2) - 1; ii < taps; ii++) {
        if (ii == (taps + 1) / 2 - 1) {
            tmp_tap = gain;
        } else {
            double tmp_t = fabs(t - 1.0 / 4.0 / beta);
            if (tmp_t < 1.0e-9) {
                sinarg = (1.0 + 2.0 / M_PI) * sin(M_PI / beta / 4.0);
                cosarg = (1.0 - 2.0 / M_PI) * cos(M_PI / beta / 4.0);
                tmp_tap = beta / sqrt(2.0 * (sinarg + cosarg));
            } else {
                sinarg = sin(M_PI * t * (1.0 - beta));
                cosarg = 4.0 * beta * t * cos(M_PI * t * (1 + beta));
                double den = M_PI * t * (1.0 - 16.0 * (beta * t) * (beta * t));
                tmp_tap = (sinarg + cosarg) / den;
            }
        }
        int indTop = ii;
        int indBot = taps - ii - 1;
        filt[indTop] = tmp_tap;
        if (ii > (taps + 1) / 2 - 1) {
            filt[indBot] = tmp_tap;
        }
        t = t + dt;
    }
    double sumFilt = 0;
    for (ii = 0; ii < N; ii++) {
        sumFilt += filt[ii];
    }
    for (ii = 0; ii < N; ii++) {
        filt[ii] /= sumFilt;
    }

    complex <double> calcFilt [N];
    for (cnt = 0; cnt < N; cnt++) {
        calcFilt[cnt].real(filt[cnt]);
    }

   for (int i = 0; i <_fftSize; i++){ //
       filter[i] = calcFilt[i];
   }
}

string DigitalSignalProcess:: GetWord() {
    return _word; // gets the words to meet Getter requirement :)
}








