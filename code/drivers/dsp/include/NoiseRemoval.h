/* 
 * File:   initFFT.h
 * Author: davi
 *
 * Created on April 2, 2012, 6:09 PM
 */

#ifndef NOISE_REMOVAL_H
#define NOISE_REMOVAL_H

#include "initFFT.h"
#include "FFT.h"

#include <bird/constants.h>
#include <bird/birdUtil.h>

#include "./dspUtil.h"

class NoiseRemoval {
public:

    NoiseRemoval();
    NoiseRemoval(int winSize, int winType);
    ~NoiseRemoval();
    int prepareSound(string pathNoiseSound, void * sampleSound,
            string pathResult);


private:

    int initNoiseRemoval(void * noise, void * sound, float *floatWavDataOut);
    bool ProcessOne(float * floatWavData, int len, float *floatWavDataOut);
    void GetProfile(int len, float *buffer);
    void RemoveNoise(int len, float *buffer);

    int windowSize;
    int windowType;
    float *noiseGate;
    float *sum;
    float *sumsq;
    float *smoothing;
    int *profileCount;
    bool doProfile;
    bool hasProfile;

    int level;

    //Parameters Default Audacity
    /* Noise Reduction = 24 dB (0 a 48) INTEGER
     * Sensivity = -0.5, 0 dB (-20.0 a +20.0) FLOAT
     * Frequency smoothing = 150 Hz (de 0 a 100) INTEGER
     * Attack/decay time = 0.15 secs (0.0 a 1.00) Float
     * Noise -> Remove (remove or isolate)
     */
};


#endif
