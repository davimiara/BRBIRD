
/*
 * 		MESTRADO COMPUTAÇÃO APLICADA

 * Autor: Davi Miara Kiapuchinski
 * Data: 10/04/2012
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

#include "../include/dspUtil.h"

int writeWavFileFFT(void* obj, string pathResult) {
    int result = ERROR;

    InitFFT* fileWav = (InitFFT*) obj;

    //transform out
    unsigned char * bufAux_8;
    unsigned short * bufAux_16 = NULL;

    //return the original value of bytes, convert the float into two bytes

    if (fileWav->getBps() == BITS_PER_SAMPLE_16) fileWav->setDataSize
            (fileWav->getDataSize() * 2);
    else if (fileWav->getBps() == BITS_PER_SAMPLE_8) fileWav->setDataSize
            (fileWav->getDataSize());

    bufAux_8 = new unsigned char[fileWav->getDataSize()];

    if (fileWav->getBps() == BITS_PER_SAMPLE_16) {
        short aux;
        unsigned char vara, varb;
        long int j = 0;
        for (long int i = 0; i < (fileWav->getDataSize() / 2); i++) {
            aux = convertFloatToByte(fileWav->getDataWav()[i]);
            vara = aux >> 8;
            varb = aux & 0xFF;
            bufAux_8[j++] = varb;
            bufAux_8[j++] = vara;
        }
    } else {
        if (fileWav->getBps() == BITS_PER_SAMPLE_8) {
            //just convert
            for (long int i = 0; i < fileWav->getDataSize(); i++) {
                bufAux_8[i] = convertFloatToByte(fileWav->getDataWav()[i]);
            }
        }
    }

    //if 16 bits - parse the little endian value
    //else if = 8 bits - just write on file

    result = createWavFile(fileWav->getDataSize(), fileWav->getRate(),
            (int)fileWav->getBps(), bufAux_8, bufAux_16, true, 
            fileWav->getFileToFFT(), pathResult);

    delete [] bufAux_8;

    return result;
}
