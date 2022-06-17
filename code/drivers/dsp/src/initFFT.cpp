/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 *   Classe responsável realização da FFT a partir de um arquivo .wav.
 *  O processo inicia com um método que extrai as informações do arquivo
 *  (channels, compression mode, rate, avg rate, bits per sample, data)
 *  e coloca taiss informações na rotina de cálculo da transformada rápida de
 *  Fourier.
 *
 * Autor: Davi Miara Kiapuchinski
 * Data: 06/02/2012
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

#include "../include/initFFT.h"

/*----------------------------------===-----------------------------------------
 *
 *                             Construtor
 *
 *----------------------------------===---------------------------------------*/
InitFFT::InitFFT() {
}

InitFFT::InitFFT(int windowType, int winSize, string fileOut) {
    setWindowType(windowType);
    setWinSize(winSize);
    setFileToFFT(fileOut);
    freqDataWav = new float[winSize/2];
    dbDataWav = new float[winSize/2];
}


InitFFT::~InitFFT() {
    if (dataWav != NULL)
        delete[] dataWav;
    if (dbDataWav != NULL)
        delete[] dbDataWav;
    if (freqDataWav != NULL)
        delete[] freqDataWav;
}

/*----------------------------------===-----------------------------------------
 *
 *                             Métodos
 *
 *----------------------------------===---------------------------------------*/

/**
 * @param parameters - parametro recebido
 * @param value - valor do parametro recebido
 * @param tablePosition - valor do indice da tabela recebido
 * @return OK or ERROR
 */
int InitFFT::openWave() {

    int result = ERROR;

    int i;

    FILE * inp;

    char riff[4];
    char wave[4];
    char fmt[4];
    char data[4];

    int nbytes;

    char stuf;
    char sbyte; /* byte of sound */

    inp = fopen(getFileToFFT().c_str(), "rb");
    if (inp == NULL) {
        printf("cannot open %s for reading. \n", getFileToFFT().c_str());
        dataWav = NULL;
        result = ERROR;
    } else {

        //setFileToFFT(fileName);

        //        printf("reading %s \n", fileName.c_str());

        int sread; /* bytes read/written */

        sread = fread(&riff[0], 1, 4, inp);
        //        printf("first 4 bytes should be RIFF, <%c%c%c%c>\n",
        //                riff[0], riff[1], riff[2], riff[3]);

        sread = fread(&totalSize, 1, 4, inp);
        //        printf("file has %d +8 bytes \n", fsize);
        setTotalSize(totalSize + 8);

        sread = fread(&wave[0], 1, 4, inp);
        //        printf("should be WAVE, <%c%c%c%c>\n", wave[0], wave[1], wave[2], wave[3]);

        sread = fread(&fmt[0], 1, 4, inp);
        //        printf("should be fmt, <%c%c%c%c>\n", fmt[0], fmt[1], fmt[2], fmt[3]);

        sread = fread(&nbytes, 1, 4, inp);
        //        printf("block has %d more bytes \n", nbytes);

        sread = fread(&compressionCode, 1, 2, inp);
        //printf("compression code = %d \n", compressionCode);
        setCompressionCode(compressionCode);
        nbytes = nbytes - 2;

        sread = fread(&channels, 1, 2, inp);
        //        printf("channels = %d \n", channels);
        setChannels(channels);
        nbytes = nbytes - 2;

        sread = fread(&rate, 1, 4, inp);
        //        printf("rate = %d  \n", rate);
        setRate(rate);
        nbytes = nbytes - 4;

        sread = fread(&avgRate, 1, 4, inp);
        //        printf("avg rate = %d \n", avgrate);
        setAvgRate(avgRate);
        nbytes = nbytes - 4;

        sread = fread(&blockAlign, 1, 2, inp);
        //        printf("blockalign = %d  \n", blockalign);
        setBlockAlign(blockAlign);
        nbytes = nbytes - 2;

        sread = fread(&bps, 1, 2, inp);
        //printf("bits per sample = %d \n", bps);
        setBps(bps);
        nbytes = nbytes - 2;
        //        printf("bytes left in fmt = %d \n", nbytes);

        for (i = 0; i < nbytes; i++) {
            sread = fread(&stuf, 1, 1, inp);
        }

        sread = fread(&data[0], 1, 4, inp);
        //printf("should be data, <%c%c%c%c>\n", data[0], data[1], data[2], data[3]);


        sread = fread(&dataSize, 1, 4, inp);
        //printf("chunk has %d more bytes \n", dataSize);
        //printf("%d bytes read so far \n", 44 + nbytes);

        if (getBps() == BITS_PER_SAMPLE_16) setDataSize(dataSize / 2);
        else if (getBps() == BITS_PER_SAMPLE_8) setDataSize(dataSize);

        dataWav = new float[getDataSize()];

        for (i = 0; i < getDataSize(); i++) {
            dataWav[i] = 0.0;
        }

        result = OK;
        if (getBps() == BITS_PER_SAMPLE_8) {
            for (i = 0; i < getDataSize(); i++) {
                sread = fread(&sbyte, 1, 1, inp);
                if (sread != 1) {
                    //printf("no read on byte %d \n", i);
                    result = ERROR;
                    break;
                } else {
                    dataWav[i] = convertByteToFloat(sbyte);
                    //                if (i < 10) {
                    //                    printf("DataByte[%d] = %c\t", i, sbyte);
                    //                    printf("DataByteBeforeConvert[%d] = %c\n", i, convertFloatToByte(dataWav[i]));
                    //                }
                }

            }
        } else {
            short dataChar[getDataSize()];
            if ((getBps() == BITS_PER_SAMPLE_16)) {
                short aux, aux2;
                unsigned char sbyteRead;
                short resultLitteEndian;
                for (long int i = 0; i < getDataSize(); i++) {

                    sread = fread(&sbyteRead, 1, 1, inp);
                    if (sread != 1) {
                        result = ERROR;
                        break;
                    } else {
                        aux = sbyteRead;
                        sread = fread(&sbyteRead, 1, 1, inp);
                        if (sread != 1) {
                            //dataWav[i] = convertByteToFloat(aux);
                            result = ERROR;
                            break;
                        } else {
                            // unity two bytes in little endian form (second number << 8 + first number)
                            aux2 = sbyteRead;
                            resultLitteEndian = (aux2 << 8) + aux;
                            dataChar[i] = resultLitteEndian;
                            dataWav[i] = (convertByteToFloat(dataChar[i]));
                        }
                    }
                }
            }
        }

        if (result != ERROR) {
            setDataWav(dataWav);
            //test a write on a file
        }

        fclose(inp);

    }

    return result;
}

/**
 * @param parameters - parametro recebido
 * @param value - valor do parametro recebido
 * @param tablePosition - valor do indice da tabela recebido
 * @return OK or ERROR
 */
int InitFFT::fastFourier() {

    int result = ERROR;

    clock_t inicio, fim;
    inicio = clock();

    float processed[getWinSize()];
    setDataFreqSize(getWinSize()/2);
    for (int i = 0; i < getWinSize(); i++)
        processed[i] = float(0.0);

    int half = getWinSize() / 2;

    float *in = new float[getWinSize()];
    float *out = new float[getWinSize()];
    float *win = new float[getWinSize()];

    // initialize the window
    for (int i = 0; i < getWinSize(); i++)
        win[i] = 1.0;
    WindowFunc(getWindowType(), getWinSize(), win);

    // Scale window such that an amplitude of 1.0 in the time domain
    // shows an amplitude of 0dB in the frequency domain
    double wss = 0;
    for (int i = 0; i < getWinSize(); i++)
        wss += win[i];
    if (wss > 0)
        wss = 4.0 / (wss * wss);
    else
        wss = 1.0;

    int start = 0;
    int windows = 0;
    while (start + getWinSize() <= getDataSize()) {

        for (int i = 0; i < getWinSize(); i++)
            in[i] = win[i] * getDataWav()[start + i];

        //WindowFunc(windowing, WIN_SIZE, in);
        //Spectrum
        PowerSpectrum(getWinSize(), in, out);
        // Take real part of result
        for (int i = 0; i < half; i++)
            processed[i] += out[i];

        start += half;
        windows++;
    }

    // Convert to decibels
    double scale;
    scale = wss / (double) windows;

    setFreqDataWav(processed);
    
//    printf("Imprimindo...\n");
//    for (int i = 0; i < half; i++) {
//        printf("processed[%d] = %f\n", i, processed[i]);
//    }

    for (int i = 0; i < half; i++) {
        processed[i] = 10 * log10(processed[i] * scale);
        //processed[i] = 10 * log10(processed[i] / WIN_SIZE / windows);
    }

    //printf("Imprimindo...\n");
    //for (int i = 1; i < half; i++) {
        //printf("frequency[%d] = %f\t", i, (float) i * getRate() / getWinSize());
        //printf("db[%d] = %f\n", i, processed[i]);
        //printf("processed[%d] = %f\n", i, processed[i]);
        //if (i % 4 == 0) printf("\n");
    //}



    delete[]in;
    delete[]out;
    delete[]win;

    fim = clock();
    printf("FFT - O tempo de execucao em segundos é %lf\n", (double) (fim - inicio) / CLOCKS_PER_SEC);
    //printf("FFT - O tempo de execucao em milisegundos é %lf\n", (double) (fim - inicio) / 1000);

    result = OK;

    return result;
}

void InitFFT::setDataSize(long int dataSize) {

    this->dataSize = dataSize;
}

long int InitFFT::getDataSize() const {

    return dataSize;
}

void InitFFT::setAvgRate(int avgrate) {

    this->avgRate = avgRate;
}

int InitFFT::getAvgRate() const {

    return avgRate;
}

void InitFFT::setRate(int rate) {

    this->rate = rate;
}

int InitFFT::getRate() {

    return rate;
}

void InitFFT::setChannels(short channels) {

    this->channels = channels;
}

short InitFFT::getChannels() const {

    return channels;
}

void InitFFT::setDataWav(float* dataWav) {

    for (long int i = 0; i < getDataSize(); i++)
        this->dataWav[i] = dataWav[i];
}

float* InitFFT::getDataWav() {

    return dataWav;
}

void InitFFT::setDbDataWav(float* dbDataWave) {
    this->dbDataWav = dbDataWave;
}

float* InitFFT::getDbDataWav() {
    return dbDataWav;
}

void InitFFT::setFreqDataWav(float* freqDataWav) {
    
    for (long int i = 0; i < getWinSize()/2; i++)
        this->freqDataWav[i] = freqDataWav[i];
    
}

float* InitFFT::getFreqDataWav() {
    return freqDataWav;
}

void InitFFT::setTotalSize(int totalSize) {

    this->totalSize = totalSize;
}

int InitFFT::getTotalSize() const {

    return totalSize;
}

void InitFFT::setBps(short bps) {

    this->bps = bps;
}

short InitFFT::getBps() const {

    return bps;
}

void InitFFT::setBlockAlign(short blockAlign) {

    this->blockAlign = blockAlign;
}

short InitFFT::getBlockAlign() const {

    return blockAlign;
}

void InitFFT::setCompressionCode(short compressionCode) {

    this->compressionCode = compressionCode;
}

short InitFFT::getCompressionCode() const {

    return compressionCode;
}

void InitFFT::setFileToFFT(string fileToFFT) {

    this->fileToFFT = fileToFFT;
}

string InitFFT::getFileToFFT() {
    return fileToFFT;
}

void InitFFT::setDataFreqSize(long int dataFreqSize) {
    this->dataFreqSize = dataFreqSize;
}

long int InitFFT::getDataFreqSize() {
    return dataFreqSize;
}

void InitFFT::setWinSize(int winSize) {
    this->winSize = winSize;
}

int InitFFT::getWinSize() const {
    return winSize;
}

void InitFFT::setWindowType(int windowType) {
    this->windowType = windowType;
}

int InitFFT::getWindowType() const {
    return windowType;
}
