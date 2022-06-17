/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 *   Classe responsável realização de filtros sobre os dados do arquivo WAV.
 *
 * Autor: Davi Miara Kiapuchinski
 * Data: 19/03/2012
 *
 * Modificações:
 *
 * ------------------------------------===--------------------------------------
 * Autor:
 * Data :
 * Alteração :
 *
 * ------------------------------------===--------------------------------------
 * Autor:
 * Data :
 * Alteração :
 * ------------------------------------===--------------------------------------
 *
 *
 */

#include "../include/filters.h"

/*----------------------------------===-----------------------------------------
 *
 *                             Construtor
 *
 *----------------------------------===---------------------------------------*/



Filters::Filters() {
}

Filters::~Filters() {
}

// the FIR filter function

int Filters::firFloat(double *coeffs, double *input, double *output,
        int length, int filterLength) {
    int result = ERROR;
    double acc; // accumulator for MACs
    double *coeffp; // pointer to coefficients
    double *inputp; // pointer to input samples
    int n;
    int k;

    // put the new samples at the high end of the buffer
    memcpy(&insamp[filterLength - 1], input,
            length * sizeof (double));

    // apply the filter to each input sample
    for (n = 0; n < length; n++) {
        // calculate output n
        coeffp = coeffs;
        inputp = &insamp[filterLength - 1 + n];
        acc = 0;
        for (k = 0; k < filterLength; k++) {
            acc += (*coeffp++) * (*inputp--);
        }
        output[n] = acc;
    }
    // shift input samples back in time for next time
    memmove(&insamp[0], &insamp[length],
            (filterLength - 1) * sizeof (double));
    return result;

}


// the FIR filter function

void Filters::firFixed(int16_t *coeffs, int16_t *input, int16_t *output,
        int length, int filterLength) {
    int32_t acc;
    // accumulator for MACs
    int16_t *coeffp; // pointer to coefficients
    int16_t *inputp; // pointer to input samples
    int n;
    int k;
    // apply the filter to each input sample
    for (n = 0; n < length; n++) {
        // calculate output n
        coeffp = coeffs;
        inputp = &input[n];
        // load rounding constant
        acc = 1 << 14;
        // perform the multiply-accumulate
        for (k = 0; k < filterLength; k++) {
            acc += (int32_t) (*coeffp++) * (int32_t) (*inputp--);
        }
        // saturate the result
        if (acc > 0x3fffffff) {
            acc = 0x3fffffff;
        } else if (acc < -0x40000000) {
            acc = -0x40000000;
        }
        // convert from Q30 to Q15
        output[n] = (int16_t) (acc >> 15);
    }
}

// FIR init

int Filters::firFloatInit(float * data, int length, float * dataOut) {

    int result = ERROR;

    memset(insamp, 0, sizeof ( insamp));

    int size, i = 0, j = 0, k = 0;
    double floatInput[SAMPLES];
    double floatOutput[SAMPLES];

    // process all of the samples
    do {
        //walk on float vector
        for (i = 0; (i < SAMPLES) && (j < length); i++) {
            floatInput[i] = data[j];
            j++;
        }
        //j++;
        size = i;

        // perform the filtering
        firFloat(coeffs, floatInput, floatOutput, size,
                FILTER_LEN);

        //store output
        for (i = 0; i < size; i++) {
            dataOut[k] = floatOutput[i];
            k++;
        }

    } while (j < length);

    result = OK;

    return result;
}

int Filters::firFloatInit(void * obj) {

    int result = ERROR;

    InitFFT* fileWav = (InitFFT*) obj;

    float * in;
    in = fileWav->getDataWav();

    float * out;
    out = new float[fileWav->getDataSize()];

    result = firFloatInit(in, fileWav->getDataSize(), out);
    result = OK;
    if (result != ERROR){
        fileWav->setDataWav(out);
        //result = writeWavFileFFT(fileWav);
    }

    delete [] out;
    
    return result;
}






