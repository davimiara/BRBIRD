/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 *   Classe responsável pelas operacoes matemáticas relacionadas a montagem
 * e aquisicao dos dados do spectograma.
 *
 * Autor: Davi Miara Kiapuchinski
 * Data: 21/10/2011
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "../include/mathDsp.h"

#define OK 0
#define ERROR -1

void intToFloat(int16_t *input, double *output, int length) {
    int i;

    for (i = 0; i < length; i++) {
        output[i] = (double) input[i];
    }
}

void floatToInt(double *input, int16_t *output, int length) {
    int i;
    for (i = 0; i < length; i++) {
        // add rounding constant
        input[i] += 0.5;
        // bound the values to 16 bits
        if (input[i] > 32767.0) {
            input[i] = 32767.0;
        } else if (input[i] < -32768.0) {
            input[i] = -32768.0;
        }
        // convert
        output[i] = (int16_t) input[i];
    }
}

float convertByteToFloat(short sByte) {
    return (float) sByte * ONEOVERSHORTMAX1;
}

short convertFloatToByte(float sFloat) {
    short out;
    float aux;
    aux = sFloat * SHORTMAX;
    out = (short) aux;
    return out;
}

