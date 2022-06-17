/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 * Autor: Davi Miara Kiapuchinski
 * Data: 20/03/2012
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
/**********************************************************************

  NoiseRemoval.cpp

  Dominic Mazzoni

  The noise is removed using noise gates on each frequency band in
  the FFT, and the signal is reconstructed using overlap/add of
  Hanning windows.

 **********************************************************************/


#include <math.h>

#include "../include/NoiseRemoval.h"

NoiseRemoval::NoiseRemoval() {
    windowSize = 2048;
    windowType = WIN_FUNCTION_HANNING;
    noiseGate = new float[windowSize];
    sum = new float[windowSize];
    sumsq = new float[windowSize];
    profileCount = new int[windowSize];
    smoothing = new float[windowSize];
    hasProfile = false;
    level = 8;
}

NoiseRemoval::NoiseRemoval(int winSize, int winType) {
    windowSize = winSize;
    windowType = winType;
    noiseGate = new float[windowSize];
    sum = new float[windowSize];
    sumsq = new float[windowSize];
    profileCount = new int[windowSize];
    smoothing = new float[windowSize];
    hasProfile = false;
    level = 8;
}

NoiseRemoval::~NoiseRemoval() {
    delete [] noiseGate;
    delete [] sum;
    delete [] sumsq;
    delete [] profileCount;
    delete [] smoothing;
}

int NoiseRemoval::prepareSound(string pathNoiseSound, void * sampleSound,
        string pathResult) {
    int result = ERROR;

    //treat noise sample
    InitFFT * ss = (InitFFT*) sampleSound;
    InitFFT * noiseWav = new InitFFT(ss->getWindowType(), ss->getWinSize(),
            pathNoiseSound);

    result = noiseWav->openWave();

    float * dataOut;
    dataOut = new float[ss->getDataSize()];
    
    if (result == OK) result = initNoiseRemoval(noiseWav, sampleSound, dataOut);

    if (result == OK) {
        ss->setDataWav(dataOut);
        result = writeWavFileFFT(ss, pathResult);
        
    }
    
    if (noiseWav != NULL) delete (noiseWav);
    if (dataOut != NULL) delete [] dataOut;
    
    return result;

}

int NoiseRemoval::initNoiseRemoval(void * noise, void * sound,
        float * floatWavDataOut) {
    int result = ERROR;

    InitFFT* sampleSound = (InitFFT*) sound;
    InitFFT* sampleNoise = (InitFFT*) noise;


    //first make the chacacteristics of noise
    doProfile = true;

    //do profile
    for (int i = 0; i < windowSize; i++) {
        sum[i] = float(0.0);
        sumsq[i] = float(0.0);
        profileCount[i] = 0;
    }
    
    clock_t inicio, fim;
    inicio = clock();
    
    //make the process
    if (!ProcessOne(sampleNoise->getDataWav(), sampleNoise->getDataSize(),
            floatWavDataOut))
        result = ERROR;
    else result = OK;

    if (result != ERROR) {
        for (int i = 0; i <= windowSize / 2; i++) {
            //float stddev = sqrt(sumsq[i] - (sum[i]*sum[i])/profileCount[i])
            //                               / profileCount[i];
            noiseGate[i] = sum[i] / profileCount[i]; // average
        }

        hasProfile = true;
    }
    if (result == OK) doProfile = false;
    //now process the real sound
    if (!ProcessOne(sampleSound->getDataWav(), sampleSound->getDataSize(),
            floatWavDataOut))
        result = ERROR;
    else result = OK;
    
    
    fim = clock();
    printf("NoiseRemoval - O tempo de execucao em segundos é %lf\n", (double) (fim - inicio) / CLOCKS_PER_SEC);
    //printf("NoiseRemoval - O tempo de execucao em milisegundos é %lf\n", (double) (fim - inicio) / 1000);

    return result;
}

bool NoiseRemoval::ProcessOne(float * floatWavData, int len,
        float *floatWavDataOut) {
    int s = 0;

    int sMaxDiskBlockSize = 1048576;
    //choise float format
    sampleFormat mSampleFormat = floatSample;
    int mMinSamples = sMaxDiskBlockSize / SAMPLE_SIZE(mSampleFormat) / 2;
    int mMaxSamples = mMinSamples * 2;

    int idealBlockLen = mMaxSamples * 4;

    if (idealBlockLen % windowSize != 0)
        idealBlockLen += (windowSize - (idealBlockLen % windowSize));

    float *bufferBlock = new float[idealBlockLen];

    float *window1 = new float[windowSize];
    float *window2 = new float[windowSize];
    float *thisWindow = window1;
    float *lastWindow = window2;

    int i;
    int start = 0;

    for (i = 0; i < windowSize; i++) {
        lastWindow[i] = 0;
        smoothing[i] = float(0.0);
    }

    while ((s < len) && ((len - s)>(windowSize / 2))) {

        int block = idealBlockLen;
        if (s + block > len)
            block = len - s;

        //Data float from sample
        //track->Get((samplePtr) buffer, floatSample, start + s, block);
        //buffer is the float data with the size equal len
        //put on buffer the values of vector initianting in start + s whith the size block
        int j = 0;
        for (int k = (start + s); k < block; k++) {
            bufferBlock[j] = floatWavData[k];
            j++;
        }
        //block is the size of sample (int)

        for (i = 0; i < (block - windowSize / 2); i += windowSize / 2) {
            int wcopy = windowSize;
            if (i + wcopy > block)
                wcopy = block - i;

            int j;
            for (j = 0; j < wcopy; j++)
                thisWindow[j] = bufferBlock[i + j];
            for (j = wcopy; j < windowSize; j++)
                thisWindow[j] = 0;

            if (doProfile)
                //make first time
                GetProfile(windowSize, thisWindow);
            else {
                //make when profile is loaded
                RemoveNoise(windowSize, thisWindow);
                for (j = 0; j < windowSize / 2; j++)
                    bufferBlock[i + j] = thisWindow[j] + lastWindow[windowSize / 2 + j];
            }

            float *tempP = thisWindow;
            thisWindow = lastWindow;
            lastWindow = tempP;
        }

        // Shift by half-a-window less than the block size we loaded
        // (so that the blocks properly overlap)
        block -= windowSize / 2;

        //write a new wav file with the result
        if (!doProfile) {
            for (int i = start + s; i < block; i++)
                floatWavDataOut[i] = bufferBlock[i];
        }

        s += block;

    }

    delete[] bufferBlock;
    delete[] window1;
    delete[] window2;

    return true;
}

void NoiseRemoval::GetProfile(int len, float *buffer) {
    float *in = new float[len];
    float *out = new float[len];

    int i;

    for (i = 0; i < len; i++)
        in[i] = buffer[i];

    // Apply window and FFT
    /* WindowFunc(3, len, in); // Hanning window */
    PowerSpectrum(len, in, out);

    for (i = 0; i <= len / 2; i++) {
        float value = log(out[i]);

        if (finite(value)) {
            sum[i] += value;
            sumsq[i] += value*value;
            profileCount[i]++;
        }
    }

    delete[] in;
    delete[] out;
}

void NoiseRemoval::RemoveNoise(int len, float *buffer) {
    float *inr = new float[len];
    float *ini = new float[len];
    float *outr = new float[len];
    float *outi = new float[len];
    float *power = new float[len];
    float *plog = new float[len];

    int i;

    for (i = 0; i < len; i++)
        inr[i] = buffer[i];

    // Apply window and FFT
    /* WindowFunc(windowType, len, inr); // Hanning window */
    FFT(len, false, inr, NULL, outr, outi);

    for (i = 0; i < len; i++)
        inr[i] = buffer[i];
    WindowFunc(windowType, len, inr); // Hanning window
    PowerSpectrum(len, inr, power);

    for (i = 0; i <= len / 2; i++)
        plog[i] = log(power[i]);

    int half = len / 2;
    for (i = 0; i <= half; i++) {
        float smooth;

        if (plog[i] < noiseGate[i] + (level / 2.0))
            smooth = float(0.0);
        else
            smooth = float(1.0);

        smoothing[i] = smooth * 0.5 + smoothing[i] * 0.5;
    }

    /* try to eliminate tinkle bells */
    for (i = 2; i <= half - 2; i++) {
        if (smoothing[i] >= 0.5 &&
                smoothing[i] <= 0.55 &&
                smoothing[i - 1] < 0.1 &&
                smoothing[i - 2] < 0.1 &&
                smoothing[i + 1] < 0.1 &&
                smoothing[i + 2] < 0.1)
            smoothing[i] = float(0.0);
    }

    outr[0] *= smoothing[0];
    outi[0] *= smoothing[0];
    outr[half] *= smoothing[half];
    outi[half] *= smoothing[half];
    for (i = 1; i < half; i++) {
        int j = len - i;
        float smooth = smoothing[i];

        outr[i] *= smooth;
        outi[i] *= smooth;
        outr[j] *= smooth;
        outi[j] *= smooth;
    }

    // Inverse FFT and normalization
    FFT(len, true, outr, outi, inr, ini);
    WindowFunc(windowType, len, inr); // Hanning window */

    for (i = 0; i < len; i++)
        buffer[i] = inr[i];

    delete[] inr;
    delete[] ini;
    delete[] outr;
    delete[] outi;
    delete[] power;
    delete[] plog;
}