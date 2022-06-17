/* 
 * File:   initFFT.h
 * Author: davi
 *
 * Created on February 6, 2012, 6:09 PM
 */

#ifndef INITFFT_H
#define	INITFFT_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

#include <linux/soundcard.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <iosfwd>
#include <math.h>
#include <sstream>
#include <iostream>

#include <stdint.h>

#include "FFT.h"

#include <bird/mathDsp.h>
#include <bird/constants.h>

#define ERROR -1
#define OK 0

typedef enum {
    int16Sample = 0x00020001,
    int24Sample = 0x00040001,
    floatSample = 0x0004000F
} sampleFormat;

#define SAMPLE_SIZE(SampleFormat) (SampleFormat >> 16)

typedef char *samplePtr;


using namespace std;

class InitFFT {
private:
    float * dataWav;
    float * freqDataWav;
    float * dbDataWav;

    short channels;
    int rate;
    int avgRate; /* average rate in samples per second */
    long int dataSize;
    long int dataFreqSize;
    int totalSize;
    short compressionCode;
    short blockAlign;
    short bps; /* bits per sample */

    int windowType;
    int winSize;

    string fileToFFT;



public:
    InitFFT();
    InitFFT(int windowType, int winSize, string fileOut);
    ~InitFFT();

    int initFFT();
    int openWave();
    int fastFourier();


    void setDataSize(long int dataSize);
    long int getDataSize() const;
    void setAvgRate(int avgrate);
    int getAvgRate() const;
    void setRate(int rate);
    int getRate();
    void setChannels(short channels);
    short getChannels() const;

    void setDataWav(float* dataWav);
    float* getDataWav();

    void setDbDataWav(float* dbDataWave);
    float* getDbDataWav();

    void setFreqDataWav(float* freqDataWav);
    float* getFreqDataWav();

    void setTotalSize(int totalSize);
    int getTotalSize() const;
    void setBps(short bps);
    short getBps() const;
    void setBlockAlign(short blockAlign);
    short getBlockAlign() const;
    void setCompressionCode(short compressionCode);
    short getCompressionCode() const;

    void setWinSize(int winSize);
    int getWinSize() const;

    void setWindowType(int windowType);
    int getWindowType() const;

    void setFileToFFT(string fileToFFT);
    string getFileToFFT();
    
    void setDataFreqSize(long int dataFreqSize);
    long int getDataFreqSize();

};

#endif	/* INITFFT_H */

