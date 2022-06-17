/* 
 * File:   charExtractor.h
 * Author: davi
 *
 * Created on March 10, 2012, 7:14 PM
 */

#ifndef CHAREXTRACTOR_H
#define	CHAREXTRACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include <sstream>

#include <time.h>

#include <bird/initFFT.h>
#include <bird/birdUtil.h>

#define ERROR -1
#define OK 0

using namespace std;

class charExtractor {
public:

private:

    FILE *file;

    //WAVE
    int dataSize;
    int dataSizeFreq;
    float * dataTime; //Data on time domain
    float * dataFreq; //Data on frequency domain
    
    //FREQUENCY
    string windowing;
    float dataFreqMean;
    float dataFreqVariance;
    float dataFreqSD; //standart deviation
    float dataFreqSDMean;
    float dataFreqSkewness;
    float dataFreqKurtosis;
    float dataFreqCentroid;
    float dataFreqSmoothness;
    float dataFreqSpread;
    float dataFreqRollOff;
    float dataFreqLoudness;
    float dataFreqFlatness;
    float dataFreqCrest;
    float dataFreqPower;

    //TIME
    float dataTimeMean;
    float dataTimeVariance;
    float dataTimeSD; //standart deviation
    float dataTimeSDMean; //average deviation
    float dataTimeSkewness;
    float dataTimeKurtosis;
    float dataTimeZCR; //zero crossing rate
    float dataTimeRMSAmplitude;
    float dataTimeLowestValue;
    float dataTimeHighestValue;
    float dataTimeSum;
    float dataTimeF0; //Fundamental Frequency
    float dataTimeNonZeroCount;

    //AMBIANCE
    string datetime;
    string local;

public:
    charExtractor();
    charExtractor(int windowType, int winSize);
    ~charExtractor();

    int createCharFile(int format, void * obj, string destPath);
    int timeChar(void * obj);
    int freqChar(void * obj);
    int waveChar(void * obj);
    int ambianceChar();
    int openWave(void * obj);

    //LOCAL and WAVE
    void setLocal(string local);
    string getLocal() const;
    void setDatetime(string datetime);
    string getDatetime() const;

    void setDataFreq(float* dataFreq);
    float* getDataFreq();
    void setDataTime(float* dataTime);
    float* getDataTime() const;
    void setDataSize(int dataSize);
    int getDataSize();
    
    
    //TIME DOMAIN
    void setDataTimeNonZeroCount(float dataTimeNonZeroCount);
    float getDataTimeNonZeroCount() const;

    void setDataTimeF0(float dataTimeF0);
    float getDataTimeF0() const;

    void setDataTimeSum(float dataTimeSum);
    float getDataTimeSum() const;

    void setDataTimeHighestValue(float dataTimeHighestValue);
    float getDataTimeHighestValue() const;

    void setDataTimeLowestValue(float dataTimeLowestValue);
    float getDataTimeLowestValue() const;

    void setDataTimeRMSAmplitude(float dataTimeRMSAmplitude);
    float getDataTimeRMSAmplitude() const;

    void setDataTimeKurtosis(float dataTimeKurtosis);
    float getDataTimeKurtosis() const;

    void setDataTimeZCR(float dataTimeZCR);
    float getDataTimeZCR() const;

    void setDataTimeSkewness(float dataTimeSkewness);
    float getDataTimeSkewness() const;

    void setDataTimeSDMean(float dataTimeSDMean);
    float getDataTimeSDMean() const;

    void setDataTimeSD(float dataTimeSD);
    float getDataTimeSD() const;

    void setDataTimeVariance(float dataTimeVariance);
    float getDataTimeVariance() const;

    void setDataTimeMean(float dataTimeMean);
    float getDataTimeMean();

    //FREQUENCY DOMAIN

    void setDataFreqLoudness(float dataFreqLoudness);
    float getDataFreqLoudness() const;

    void setDataFreqPower(float dataFreqPower);
    float getDataFreqPower() const;

    void setDataFreqCrest(float dataFreqCrest);
    float getDataFreqCrest() const;

    void setDataFreqFlatness(float dataFreqFlatness);
    float getDataFreqFlatness() const;

    void setDataFreqRollOff(float dataFreqRollOff);
    float getDataFreqRollOff() const;

    void setDataFreqSpread(float dataFreqSpread);
    float getDataFreqSpread() const;

    void setDataFreqSmoothness(float dataFreqSmoothness);
    float getDataFreqSmoothness() const;

    void setDataFreqSkewness(float dataFreqSkewness);
    float getDataFreqSkewness() const;

    void setDataFreqCentroid(float dataFreqCentroid);
    float getDataFreqCentroid() const;

    void setDataFreqKurtosis(float dataFreqKurtosis);
    float getDataFreqKurtosis() const;

    void setDataFreqSDMean(float dataFreqSDMean);
    float getDataFreqSDMean() const;

    void setDataFreqSD(float dataFreqSD);
    float getDataFreqSD() const;

    void setDataFreqVariance(float dataFreqVariance);
    float getDataFreqVariance() const;

    void setDataFreqMean(float dataFreqMean);
    float getDataFreqMean() const;

    void setWindowing(string windowing);
    string getWindowing() const;
    
    void setDataSizeFreq(int dataSizeFreq);
    int getDataSizeFreq();



};


#endif	/* CHAREXTRACTOR_H */