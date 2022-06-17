/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 *   Classe responsável manutenção do microfone do HW de desenvolvimento
 *
 * Autor: Davi Miara Kiapuchinski
 * Data: 15/07/2011
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

#include "../include/microphone.h"

/*----------------------------------===-----------------------------------------
 *
 *                             Construtor
 *
 *----------------------------------===---------------------------------------*/

Microphone::Microphone() {
    this->setChannels(MIC_CHANNELS);
    this->setLength(MIC_LENGTH);
    this->setRate(MIC_RATE_22050);
    this->setBps(WORD_SIZE);
    this->setDataSize(SIZE_BUFFER*MIC_RATE_22050);

    if (getBps() == BITS_PER_SAMPLE_16) {
        //            bufferMic_16 = new unsigned short[SIZE_BUFFER];
        bufferMic_8 = new unsigned char[getDataSize()];
    } else if (getBps() == BITS_PER_SAMPLE_8) {
        bufferMic_8 = new unsigned char[getDataSize()];
    }
}

Microphone::Microphone(float length, int rate, int bps, int channels) {
    this->setChannels(channels);
    this->setLength(length);
    this->setRate(rate);
    this->setBps(bps);
    this->setDataSize(getLength() * getRate() * getBps() * getChannels() / 8);
    
//    SIZE_BUFFER MIC_LENGTH*MIC_RATE*WORD_SIZE*MIC_CHANNELS/(8)

    if (getBps() == BITS_PER_SAMPLE_16) {
        //            bufferMic_16 = new unsigned short[SIZE_BUFFER];
        bufferMic_8 = new unsigned char[getDataSize()];
    } else if (getBps() == BITS_PER_SAMPLE_8) {
        bufferMic_8 = new unsigned char[getDataSize()];
    }
}

Microphone::~Microphone() {

    if (getBps() == BITS_PER_SAMPLE_16) {
        //delete [] bufferMic_16;
        delete [] bufferMic_8;
    } else if (getBps() == BITS_PER_SAMPLE_8) {
        delete [] bufferMic_8;
    }
}

/*----------------------------------===-----------------------------------------
 *
 *                             Métodos
 *
 *----------------------------------===---------------------------------------*/

/**
 * Prepara os dados para serem gravados
 * @param parameters - parametro recebido
 * @param value - valor do parametro recebido
 * @param tablePosition - valor do indice da tabela recebido
 * @return OK or ERROR
 */
int Microphone::init() {

    int fd; /* sound device file descriptor */
    int status; /* return status of system calls */
    //printf("Testando canal...\n");

    /* open sound device */
    fd = open(MIC_PATH, O_WRONLY);
    if (fd < 0) {
        perror("open of /dev/dsp failed\n");
        status = ERROR;
    }

    status = configureMic(fd);
    close(fd);

    return status;

}

//configure MIC

int Microphone::configureMic(int fd) {

    //printf("Configurando...\n");

    int result = ERROR;
    int arg;

    /* set sampling parameters */
    arg = getBps(); /* sample size */
    result = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
    if (result == ERROR)
        printf("SOUNDSIZE_PCM_WRITE_BITS ioctl failed\n");
    if (arg != getBps()) {
        printf("unable to set sample bps\n");
        result = ERROR;
    }

    arg = getChannels(); /* mono or stereo */
    result = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (result == ERROR)
        printf("SOUND_PCM_WRITE_CHANNELS ioctl failed\n");
    if (arg != getChannels()) {
        printf("unable to set number of channels\n");
        result = ERROR;
    }

    arg = getRate(); /* sampling rate */
    result = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
    if (result == ERROR)
        printf("SOUND_PCM_WRITE_WRITE ioctl failed\n");

    return result;
}

int Microphone::record() {

    int result = 0;
    int fd;

    fd = open(MIC_PATH, O_RDONLY);

    if (fd < 0) {
        printf("open of /dev/dsp failed\n");
        result = ERROR;
    } else {
        if (configureMic(fd) == OK) {
            printf("REC...with bps: %d, rate: %d, totalSize: %d\n", getBps(),
                    getRate(), getDataSize());

            if (getBps() == BITS_PER_SAMPLE_8) {
                /* record some sound */
                result = read(fd, bufferMic_8, getDataSize());
                if (result != getDataSize()) {
                    printf("read wrong number of bytes_8\n");
                    result = ERROR;
                } else {
                    //store buf
                    //setBufferMic_8(bufferMic_8);
                }

            } else if (getBps() == BITS_PER_SAMPLE_16) {
                /* record some sound */
                result = read(fd, bufferMic_8, getDataSize());
                if (result != getDataSize()) {
                    printf("read wrong number of bytes_16\n");
                    result = ERROR;
                } else {
                    //store buf
                    //setBufferMic_16(bufferMic_16);
                }
            }
        }
    }

    close(fd);
    return result;
}

int Microphone::playSound() {

    int result = ERROR;
    try {
        int fd;
        fd = open(MIC_PATH, O_WRONLY);
        if (fd < 0) {
            printf("open of /dev/dsp failed\n");
            result = ERROR;
        } else {
            if (configureMic(fd) == OK) {
                printf("You said:\n");
                if (getBps() == BITS_PER_SAMPLE_8) {
                    result = write(fd, bufferMic_8, getDataSize());

                    if (result != getDataSize()) {
                        printf("wrote wrong number of bytes");
                        result = ERROR;
                    } else {
                        /* Wait until all bytes been sent */
                        result = ioctl(fd, SOUND_PCM_SYNC, 0);
                        if (result == ERROR)
                            printf("SOUND_PCM_SYNC ioctl failed");
                        else result = OK;
                    }
                } else if (getBps() == BITS_PER_SAMPLE_16) {
                    result = write(fd, bufferMic_8, getDataSize());

                    if (result != getDataSize()) {
                        printf("wrote wrong number of bytes");
                        result = ERROR;
                    } else {
                        /* Wait until all bytes been sent */
                        result = ioctl(fd, SOUND_PCM_SYNC, 0);
                        if (result == ERROR)
                            printf("SOUND_PCM_SYNC ioctl failed");
                        else result = OK;
                    }
                }
            }
        }
        close(fd);
    } catch (...) {

    }
    return result;

}

int Microphone::writeWavFile(string destination) {
    int result = ERROR;

    //transform out
    unsigned char * bufAux;

    bufAux = new unsigned char[getDataSize()];

//    long int j = 0;
//    //invert the order of bytes to store on litten endian format
//    for (long int i = 0; i < getDataSize(); i++) {
//        bufAux[j++] = bufferMic_8[i + 1];
//        bufAux[j++] = bufferMic_8[i];
//        i++;
//    }

    //    if (getBps() == BITS_PER_SAMPLE_16) {
    result = createWavFile(getDataSize(), getRate(), getBps(),
            bufferMic_8, bufferMic_16, false, "", destination);
    //    } else if (getBps() == BITS_PER_SAMPLE_8) {
    //        result = createWavFile(getDataSize(), getRate(), getBps(),
    //                bufferMic_8, bufferMic_16, false, "", WAV_OUT_MIC_PATH);
    //    }



    //if 16 bits - parse the little endian value

    //else if = 8 bits - just write on file



    delete [] bufAux;

    return result;
}

void Microphone::setChannels(int channels) {

    this->channels = channels;
}

int Microphone::getChannels() {

    return channels;
}

void Microphone::setBps(int bps) {
    //printf("Setting BPS: %d", bps);
    this->bps = bps;
}

int Microphone::getBps() {
    return bps;
}

void Microphone::setRate(int rate) {

    this->rate = rate;
}

int Microphone::getRate() {

    return rate;
}

void Microphone::setLength(float length) {

    this->length = length;
}

float Microphone::getLength() {
    return length;
}

//void Microphone::setBufferMic_8(unsigned char bufferMic_8) {
//    this->bufferMic_8 = bufferMic_8;
//}
//
//unsigned char Microphone::getBufferMic_8() const {
//    return bufferMic_8;
//}
//
//void Microphone::setBufferMic_16(unsigned short bufferMic_16) {
//    this->bufferMic_16 = bufferMic_16;
//}
//
//unsigned short Microphone::getBufferMic_16() const {
//    return bufferMic_16;
//}

void Microphone::setDataSize(int dataSize) {
    this->dataSize = dataSize;
}

int Microphone::getDataSize() {
    return dataSize;
}