/*
 * File:   main.cpp
 * Author: davi
 *
 * Created on July 20, 2011, 7:24 PM
 */

/*
 * player.c
 * Program to illustrate /dev/dsp device
 * Records several seconds of sound, then echoes it back.
 * Runs until Control-C is pressed.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <fstream>

using namespace std;

#define LENGTH 3   /* how many seconds of speech to store */
#define RATE 8000   /* the sampling rate */
#define SIZE 8      /* sample size: 8 or 16 bits */
#define CHANNELS 1  /* 1 = mono 2 = stereo */

/* this buffer holds the digitized audio */
unsigned char buf[LENGTH*RATE*SIZE*CHANNELS / 8];

int main() {

    int fd; /* sound device file descriptor */
    int arg; /* argument for ioctl calls */
    int status; /* return status of system calls */

    long size;
    char char1, char2;
    char * buffer;

    unsigned char chunkID[4] = {'R', 'I', 'F', 'F'};
    unsigned char chunkSize[4] = {0xe4, 0x5d, 0x00, 0x00}; //36+Subchunk2Size
    unsigned char format[4] = {'W', 'A', 'V', 'E'};
    unsigned char subchunk1ID[4] = {'f', 'm', 't', 0x20};

    unsigned char subchunk1Sizet[4] = {0x10, 0x00, 0x00, 0x00}; //This is the size of the rest of the Subchunk which follows this number.
    unsigned char audioFormat[2] = {0x01, 0x00}; //PCM = 1 (i.e. Linear quantization)
    unsigned char numChannels[2] = {0x01, 0x00}; //Mono = 1, Stereo = 2, etc.
    unsigned char sampleRate[4] = {0x40, 0x1F, 0x00, 0x00}; //8000, 44100, etc.

    unsigned char byteRate[4] = {0x40, 0x1F, 0x00, 0x00}; //== SampleRate * NumChannels * BitsPerSample/8
    unsigned char blockAlign[2] = {0x01, 0x00};
    unsigned char bitsPerSample[2] = {0x08, 0x00}; //8 bits = 8, 16 bits = 16, etc.

    unsigned char Subchunk2ID[4] = {0x64, 0x61, 0x74, 0x61}; //data

    unsigned char Subchunk2Size[4] = {0xc0, 0x4d, 0x00, 0x00}; //== NumSamples * NumChannels * BitsPerSample/8 ->only sample on this case

    FILE *file;
    FILE *dataSound;
    file = fopen("/home/davi/Mestrado/cpp/Bird/drivers/microphone/sound1.wav", "w+");

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

    ifstream infile("/home/davi/Mestrado/cpp/waveFile/aux.wav", ifstream::binary);
    // get size of file
    infile.seekg(0, ifstream::end);
    size = infile.tellg();
    printf("SiZe: %ld\n", size);
    infile.seekg(0);
    // allocate memory for file content
    buffer = new char [size];

    /*A .WAV file is a file that must respect a header and need to be written
     on a litte endian form. So it's necessary to invert the bits on some
     * header fields and on data */

    while (infile.good()) // loop while extraction from file is possible
    {
        char1 = infile.get(); // get character from file
        if (infile.good()) {
            char2 = infile.get(); // get character from file
            if (infile.good()) {
                // write to outfile
                fputc(char2, file);
                fputc(char1, file);
            } else {
                fputc(char1, file);
            }
        } else {

        }

    }


    // release dynamically-allocated memory
    if (buffer != NULL) delete[] buffer;

    infile.close();
    fclose(file); /*done!*/
    return 0;

}