/* 
 * File:   main.cpp
 * Author: davi
 *
 * Created on July 18, 2011, 9:00 AM
 */

#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <time.h>

#include <bird/initFFT.h>
#include <bird/charExtractor.h>
#include <bird/microphone.h>
#include <bird/filters.h>
#include <bird/NoiseRemoval.h>

#include <bird/dspUtil.h>
#include <bird/constants.h>

#define OK 0
#define ERROR -1

using namespace std;

int extractChar(void * obj) {
    int result = ERROR;
    charExtractor * ce = new charExtractor();
    result = ce->createCharFile(1, obj, "");
    if (ce != NULL) delete (ce);
    return result;
}

int digSignP(void * obj, string outPath, int windowSize, int windowType,
        string pathResult) {
    int result = ERROR;

    //apply noise reduction
    NoiseRemoval * nr = new NoiseRemoval(windowSize, windowType);
    result = nr->prepareSound(outPath, obj, pathResult);
    if (nr != NULL) delete (nr);

    return result;
}

int record(int length, int rate, int bps, int channels, string outPath) {

    int result = ERROR;


    Microphone * mic = new Microphone(length, rate, bps, channels);

    result = mic->init();

    if (result == OK) result = mic->record();
    else printf("Error on rec sound\n");

    if (result != ERROR) result = mic->playSound();
    if (result != OK) printf("Error on play the sound\n");
    else {
        //save a wav file
        mic->writeWavFile(outPath);
    }

    if (mic != NULL) delete (mic);

    return result;

}

//teste routine - use the store sound to apply other types of tests

int testRoutine(int testId, string pathSound, string pathNoise,
        string pathResult, int rate, int winFunction, int winSize) {

    int result = ERROR;
    //change win type
    //change sample rate
    //change win size

    printf("TESTE %d: Tx de Amostragem %d, WinSize: %d, WinType: %d\n", testId,
            rate, winSize, winFunction);
    InitFFT * fileWav = new InitFFT(winFunction, winSize, pathSound);
    result = extractChar(fileWav);
    if (result != OK) {
        printf("testeFAIL(ExtractChar)\n");
        return result;
    } else {
        printf("testeOK (ExtractChar)\n");
    }
    result = digSignP(fileWav, pathNoise, winSize, winFunction, pathResult);
    if (fileWav != NULL) delete (fileWav);
    if (result == OK) printf("testeOK (NoiseReduction)\n");
    else printf("testeFAIL (NoiseReduction)\n");

    return result;
}

int initBird() {
    int result;

    int length = MIC_LENGTH;
    int bps = MIC_SIZE_16;
    int channels = MIC_CHANNELS;
    string pathNoise;
    string pathOut;
    string pathResult;
    
    //pathOut.assign(WAV_OUT_MIC_PATH);
    pathOut.assign("/home/davi/waves/3.wav");
    pathResult.assign(WAV_OUT_PATH);
    pathNoise.assign("/home/davi/waves/noise3.wav");

    printf("Arquivo de ruido: %s\n", pathNoise.c_str());
    printf("Arquivo de gravação: %s\n", pathOut.c_str());
    printf("Arquivo resultante: %s\n", pathResult.c_str());

    //REC a sound
    //result = record(length, MIC_RATE_22050, bps, channels, pathOut);
    //teste routine
    result = testRoutine(1, pathOut, pathNoise, pathResult, MIC_RATE_22050, WIN_FUNCTION_HANNING, WIN_SIZE_1024);
    
    return result;
}

/*record
 * 
 */
int main() {
    int result;

    result = initBird();

    result = OK;
    return result;
}