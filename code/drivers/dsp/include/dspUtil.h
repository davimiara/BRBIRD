/* 
 * File:   dspUtil.h
 * Author: davi
 *
 * Created on April 10, 2012, 5:29 PM
 */

#ifndef DSPUTIL_H
#define	DSPUTIL_H

#define ERROR -1
#define OK 0

//#include <bird/microphone.h>

#include "initFFT.h"
#include <bird/birdUtil.h>

int writeWavFileFFT(void * obj, string pathResult);

#endif	/* DSPUTIL_H */

