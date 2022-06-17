/* 
 * File:   BIRDUtil.h
 * Author: davi
 *
 * Created on March 31, 2012, 12:29 AM
 */

#ifndef BIRDUTIL_H
#define	BIRDUTIL_H

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

#include "mathDsp.h"
#include "constants.h"

using namespace std;

int getHeaderWav(string fileName, char * aux);
int writeHeaderWav(FILE * file, long int sizeBuffer, int micRate);
int writeWavData_8(FILE * file, long int sizeBuffer, unsigned char * data);
int writeWavData_16(FILE * file, long int sizeBuffer, unsigned short * data);
int createWavFile();
int createWavFile(long int sizeBuffer, int micRate, int bps,
        unsigned char * data_8, unsigned short * data_16, bool isHeaderReady,
        string fileIn, string fileOut);
int parseNumber(int dec, int positionByte);
string getWindowName(int windowType);

#endif	/* BIRDUTIL_H */

