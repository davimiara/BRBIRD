/* 
 * File:   microphone.h
 * Author: davi
 *
 * Created on July 21, 2011, 8:12 AM
 */

#ifndef MICROPHONE_H
#define	MICROPHONE_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>

#include <linux/soundcard.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <iosfwd>
#include <math.h>
#include <sstream>
#include <iostream>

#include <bird/birdUtil.h>
#include <bird/constants.h>

#define ERROR -1
#define OK 0

/* this buffer holds the digitized audio */


using namespace std;

class Microphone {
    
private:
    float length; //second to recorder
    int rate;
    int bps;
    int channels;
    int dataSize;

    FILE *file;
    
    unsigned char * bufferMic_8;
    unsigned short * bufferMic_16;

public:
    Microphone();
    Microphone(float length, int rate, int bps, int channels); // parametersMic
    ~Microphone();

public:

    int record();
    int init(); //open Channel
    int configureMic(int fd);
    int playSound(); //close Channel
    int writeWavFile(string destination);

    void setChannels(int channels);
    int getChannels();
    void setBps(int bps);
    int getBps();
    void setRate(int rate);
    int getRate();
    void setLength(float length);
    float getLength();
    void setBufferMic_8(unsigned char bufferMic_8);
    unsigned char getBufferMic_8() const;
    void setBufferMic_16(unsigned short bufferMic_16);
    unsigned short getBufferMic_16() const;
    void setDataSize(int dataSize);
    int getDataSize();

};

#endif	/* MICROPHONE_H */

