/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 *   Bloco de código responsável em realizar operações uteis a todas as classes
 * do software
 *
 * Autor: Davi Miara Kiapuchinski
 * Data: 31/03/2012
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

#include "../include/birdUtil.h"

int getHeaderWav(string fileName, char * aux) {

    int result = ERROR;

    FILE * inp;

    char headerWav[WAV_HEADER_LENGTH];

    inp = fopen(fileName.c_str(), "rb");
    if (inp == NULL) {
        //printf("cannot open %s for reading. \n", fileName.c_str());
        result = ERROR;
    } else {
        //        int sread = 0; /* bytes read/written */
        //GO TO THE START of the header
        //rewind(inp);
        //save the header 44 bytes
        fread(headerWav, 1, WAV_HEADER_LENGTH, inp);
        fclose(inp);
        result = OK;
    }
    //    printf("Cabeçalho: \n");
    for (int i = 0; i < WAV_HEADER_LENGTH; i++) {
        //printf("%c", headerWav[i]);
        aux[i] = headerWav[i];
    }

    result = OK;

    return result;
}

int writeHeaderWav(FILE * file, long int sizeBuffer, int micRate) {
    int result = ERROR;

    unsigned char chunkID[4] = WAV_HEADER1;

    //unsigned char chunkSize[4] = {0xe4, 0x5d, 0x00, 0x00}; //36+Subchunk2Size
    unsigned char chunkSize[4]; //36+Subchunk2Size
    chunkSize[3] = parseNumber(sizeBuffer + 36, BYTE_PART4);
    chunkSize[2] = parseNumber(sizeBuffer + 36, BYTE_PART3);
    chunkSize[1] = parseNumber(sizeBuffer + 36, BYTE_PART2);
    chunkSize[0] = parseNumber(sizeBuffer + 36, BYTE_PART1);

    unsigned char format[4] = WAV_HEADER2;
    unsigned char subchunk1ID[4] = WAV_HEADER3;

    //This is the size of the rest of the Subchunk which follows this number.
    unsigned char subchunk1Sizet[4] = {0x10, 0x00, 0x00, 0x00};

    //PCM = 1 (i.e. Linear quantization)
    unsigned char audioFormat[2] = {0x01, 0x00};

    //unsigned char numChannels[2] = {0x01, 0x00}; //Mono = 1, Stereo = 2, etc.
    unsigned char numChannels[2]; //Mono = 1, Stereo = 2, etc.
    numChannels[1] = parseNumber(MIC_CHANNELS, BYTE_PART2);
    numChannels[0] = parseNumber(MIC_CHANNELS, BYTE_PART1);

    //unsigned char sampleRate[4] = {0x40, 0x1F, 0x00, 0x00}; //8000, 44100, etc.
    unsigned char sampleRate[4]; //8000, 44100, etc.
    sampleRate[3] = parseNumber(micRate, BYTE_PART4);
    sampleRate[2] = parseNumber(micRate, BYTE_PART3);
    sampleRate[1] = parseNumber(micRate, BYTE_PART2);
    sampleRate[0] = parseNumber(micRate, BYTE_PART1);

    //== SampleRate * NumChannels * BitsPerSample/8
    //unsigned char byteRate[4] = {0x40, 0x1F, 0x00, 0x00};
    unsigned char byteRate[4];
    byteRate[3] = parseNumber(micRate * MIC_CHANNELS * WORD_SIZE / ONE_BYTE, BYTE_PART4);
    byteRate[2] = parseNumber(micRate * MIC_CHANNELS * WORD_SIZE / ONE_BYTE, BYTE_PART3);
    byteRate[1] = parseNumber(micRate * MIC_CHANNELS * WORD_SIZE / ONE_BYTE, BYTE_PART2);
    byteRate[0] = parseNumber(micRate * MIC_CHANNELS * WORD_SIZE / ONE_BYTE, BYTE_PART1);

    unsigned char blockAlign[2] = {0x02, 0x00};

    //8 bits = 8, 16 bits = 16, etc.
    //unsigned char bitsPerSample[2] = {0x08, 0x00};
    unsigned char bitsPerSample[2];
    bitsPerSample[1] = parseNumber(WORD_SIZE, BYTE_PART2);
    bitsPerSample[0] = parseNumber(WORD_SIZE, BYTE_PART1);


    unsigned char Subchunk2ID[4] = {0x64, 0x61, 0x74, 0x61}; //data

    //== NumSamples * NumChannels * BitsPerSample/8 ->only sample on this case
    //unsigned char Subchunk2Size[4] = {0xc0, 0x5d, 0x00, 0x00};
    unsigned char Subchunk2Size[4];
    Subchunk2Size[3] = parseNumber(sizeBuffer, BYTE_PART4);
    Subchunk2Size[2] = parseNumber(sizeBuffer, BYTE_PART3);
    Subchunk2Size[1] = parseNumber(sizeBuffer, BYTE_PART2);
    Subchunk2Size[0] = parseNumber(sizeBuffer, BYTE_PART1);

    fwrite(chunkID, 1, 4, file);
    fwrite(chunkSize, 1, 4, file);
    fwrite(format, 1, 4, file);
    fwrite(subchunk1ID, 1, 4, file);
    fwrite(subchunk1Sizet, 1, 4, file);
    fwrite(audioFormat, 1, 2, file);
    fwrite(numChannels, 1, 2, file);
    fwrite(sampleRate, 1, 4, file);
    fwrite(byteRate, 1, 4, file);
    fwrite(blockAlign, 1, 2, file);
    fwrite(bitsPerSample, 1, 2, file);
    fwrite(Subchunk2ID, 1, 4, file);
    fwrite(Subchunk2Size, 1, 4, file);

    result = OK;

    return result;
}

int writeWavData_8(FILE * file, long int sizeBuffer, unsigned char * data) {
    int result = ERROR;
    for (long int i = 0; i < sizeBuffer; i++) {// loop while extraction from file is possible
        fputc(data[i], file);
    }
    result = OK;
    return result;
}

int writeWavData_16(FILE * file, long int sizeBuffer, unsigned short * data) {
    int result = ERROR;
    fwrite(data, 2, sizeBuffer, file);
    result = OK;
    return result;
}

int createWavFile(float * dataWavFile, int rate) {
    int result = ERROR;
    int sizeBuffer = SIZE_BUFFER * rate;
    int micRate = rate;
    int sizeWord = WORD_SIZE;

    unsigned char * data_8 = NULL;
    unsigned short * data_16 = NULL;

    if (sizeWord == BITS_PER_SAMPLE_8)
        data_8 = new unsigned char[sizeBuffer];
    else if (sizeWord == BITS_PER_SAMPLE_16)
        data_16 = new unsigned short[sizeBuffer];

    //with no header - mount on function
    result = createWavFile(sizeBuffer, micRate, sizeWord,
            data_8, data_16, false, "", "");

    if (sizeWord == BITS_PER_SAMPLE_8)
        delete [] data_8;
    else if (sizeWord == BITS_PER_SAMPLE_16)
        delete [] data_16;

    return result;

}

//Write full header or mount it

int createWavFile(long int sizeBuffer, int micRate, int bps,
        unsigned char * data_8, unsigned short * data_16, bool isHeaderReady,
        string fileIn, string fileOut) {

    int result = ERROR;
    FILE * file;
    if (!strcmp(fileOut.c_str(), ""))
        file = fopen(WAV_OUT_PATH, "w");
    else
        file = fopen(fileOut.c_str(), "w");

    if (!isHeaderReady) {
        result = writeHeaderWav(file, sizeBuffer, micRate);
    } else {
        //take header from the wav file
        char * aux;
        aux = new char[WAV_HEADER_LENGTH];
        getHeaderWav(fileIn, aux);
        if (strcmp(aux, "")) {
            fwrite(aux, 1, WAV_HEADER_LENGTH, file);
            result = OK;
        } else result = ERROR;
        delete [] aux;
    }

    //write for word of 8 bits and 16 bits
    if (result == OK) {
        //        if (bps == BITS_PER_SAMPLE_8){
        writeWavData_8(file, sizeBuffer, data_8);
        //        }else{
        //            if (bps == BITS_PER_SAMPLE_16){
        //                writeWavData_16(file, sizeBuffer, data_16);
        //            }
        //        }
        //
    }

    result = OK;
    fclose(file); /*done!*/

    //    FILE *file1;
    //    file1 = fopen("aux.wav", "a+");
    //    fwrite(buf, 1, sizeof (buf), file1); //writting data
    //    fclose(file1); /*done!*/

    return result;
}

/**
 * @param  positionByte - posicao do byte no numero a ser retornada
 * do numero ou não
 * @return OK or ERROR
 */
int parseNumber(int dec, int positionByte) {

    int aux = 0;
    //shiftleft
    //aux = dec >> 8; //Ex: 0x5DC0 -> 0x5D
    //binValue << hex << aux;

    switch (positionByte) {
        case 1:
        {
            //Take the First 8 bits
            aux = dec & 0x000000FF;
        }
            break;
        case 2:
        {
            aux = dec & 0x0000FF00; //Ex: 0x5DC0 -> 0x5DbinValue << hex << aux;
            aux = aux >> 8;
        }
            break;
        case 3:
        {
            aux = dec & 0x00FF0000; //Ex: 0x5DC0 -> 0x5DbinValue << hex << aux;
            aux = aux >> 16;
        }
            break;
        case 4:
        {
            aux = dec & 0xFF000000; //Ex: 0x5DC0 -> 0x5DbinValue << hex << aux;
            aux = aux >> 24;
        }
            break;
        default: aux = ERROR;
            break;
    }

    return aux < 0 ? ERROR : aux;
}

string getWindowName(int windowType) {
    string result = "ERROR";

    switch (windowType) {
        case WIN_FUNCTION_BARTLETT:
        {
            result = WIN_FUNCTION_BARTLETT_NAME;
        }
        break;
        case WIN_FUNCTION_HAMMING:
        {
            result = WIN_FUNCTION_HAMMING_NAME;
        }
        break;
        case WIN_FUNCTION_HANNING:
        {
            result = WIN_FUNCTION_HANNING_NAME;
        }
        break;
        default: result = "ERROR";
    }

    return result;
}