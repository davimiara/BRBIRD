/* 
 * File:   math.h
 * Author: davi
 *
 * Created on August 9, 2011, 1:04 PM
 */

#ifndef MATHDSP_H
#define	MATHDSP_H

#define SWAP(a,b) int temp=(a) ; (a) = (b) ; (b) = tempr
#define PI 3.1415
#define ONEOVERSHORTMAX 3.0517578125e-5f // 1/32768
#define ONEOVERSHORTMAX1 1/32768
#define ONEOVERSHORTMAX2 1/65536
#define ONEOVERSHORTMAX3 1/1024
#define SHORTMAX 32768.0 //0x8000
#define OK 0
#define ERROR -1

float convertByteToFloat(short sByte);
short convertFloatToByte(float sFloat);

void intToFloat(int16_t *input, double *output, int length);
void floatToInt( double *input, int16_t *output, int length );


#endif	/* MATH_H */

