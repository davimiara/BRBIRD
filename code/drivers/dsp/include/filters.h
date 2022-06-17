/* 
 * File:   filters.h
 * Author: davi
 *
 * Created on August 11, 2011, 2:28 PM
 */

#ifndef FILTERS_H
#define	FILTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string>
#include <string.h>

#include <bird/birdUtil.h>
#include <bird/microphone.h>

#include "./dspUtil.h"
#include "initFFT.h"

#define OK 0
#define ERROR -1


//////////////////////////////////////////////////////////////
//  Filter Code Definitions
//////////////////////////////////////////////////////////////

// maximum number of inputs that can be handled
// in one function call
#define MAX_INPUT_LEN 80
// maximum length of filter than can be handled
#define MAX_FLT_LEN 63
// buffer to hold all of the input samples
#define BUFFER_LEN (MAX_FLT_LEN - 1 + MAX_INPUT_LEN)

#define SAMPLES 80

// bandpass filter centred around 1000 Hz
// sampling rate = 8000 Hz

#define FILTER_LEN  63
double coeffs[ FILTER_LEN ] ={
    -0.0026050, 0.0009204, 0.0025663, 0.0042732, 0.0049130, 0.0037406, 0.0009114,
    -0.0024392, -0.0047209, -0.0048182, -0.0029059, -0.0005869, -0.0000925, -0.0028341,
    -0.0080844, -0.0126822, -0.0122418, -0.0035407, 0.0130262, 0.0323470, 0.0458351,
    0.0448912, 0.0252654, -0.0098125, -0.0494745, -0.0787035, -0.0842895, -0.0607875,
    -0.0137423, 0.0415640, 0.0856394, 0.1023991, 0.0856394, 0.0415640, -0.0137423,
    -0.0607875, -0.0842895, -0.0787035, -0.0494745, -0.0098125, 0.0252654, 0.0448912,
    0.0458351, 0.0323470, 0.0130262, -0.0035407, -0.0122418, -0.0126822, -0.0080844,
    -0.0028341, -0.0000925, -0.0005869, -0.0029059, -0.0048182, -0.0047209, -0.0024392,
    0.0009114, 0.0037406, 0.0049130, 0.0042732, 0.0025663, 0.0009204, -0.0026050
};

// array to hold input samples
double insamp[ BUFFER_LEN ];

class Filters {
public:
    Filters();
    ~Filters();

    int firFloat(double *coeffs, double *input, double *output,
            int length, int filterLength);
    void firFixed(int16_t *coeffs, int16_t *input, int16_t *output,
            int length, int filterLength);
    int firFloatInit(void * obj);
    int firFloatInit(float * data, int length, float * dataOut);

};

#endif	/* FILTERS_H */

