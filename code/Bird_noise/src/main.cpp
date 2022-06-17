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

#include <bird/microphone.h>

#include <bird/dspUtil.h>
#include <bird/constants.h>

#define OK 0
#define ERROR -1

using namespace std;

int recordNoise(float size, int rate, string path) {

    int result = ERROR;
    Microphone * mic = new Microphone(size, rate,
            MIC_SIZE_16_NOISE, MIC_CHANNELS_NOISE);
    result = mic->init();
    if (result == OK) result = mic->record();
    else printf("Error on rec sound Noise\n");
    if (result != ERROR) result = mic->playSound();
    if (result != OK) printf("Error on play Noise sound\n");
    else {
        //save a wav file
        mic->writeWavFile(path.c_str());
    }
    if (mic != NULL) delete (mic);

    return result;
}

int prepareRecordNoise() {

    int result = ERROR;

    int rate;
    float size;

    string path;

    //#define SIZE_BUFFER MIC_LENGTH*WORD_SIZE*MIC_CHANNELS/(8)

    printf("Amostra ruido com amostragem 8000 Hz e 1seg\n");
    rate = MIC_RATE_NOISE_8000;
    size = MIC_LENGTH_NOISE_1;
    path.assign(NOISE_OUT_MIC_PATH_1s_8000);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 16000 Hz e 1seg\n");
    rate = MIC_RATE_NOISE_16000;
    size = MIC_LENGTH_NOISE_1;
    path.assign(NOISE_OUT_MIC_PATH_1s_16000);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 22050 Hz e 1seg\n");
    rate = MIC_RATE_NOISE_22050;
    size = MIC_LENGTH_NOISE_1;
    path.assign(NOISE_OUT_MIC_PATH_1s_22050);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 44100 Hz e 1seg\n");
    rate = MIC_RATE_NOISE_44100;
    size = MIC_LENGTH_NOISE_1;
    path.assign(NOISE_OUT_MIC_PATH_1s_44100);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 8000 Hz e 0.5seg\n");
    rate = MIC_RATE_NOISE_8000;
    size = MIC_LENGTH_NOISE_05;
    path.assign(NOISE_OUT_MIC_PATH_05s_8000);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 16000 Hz e 0.5seg\n");
    rate = MIC_RATE_NOISE_16000;
    size = MIC_LENGTH_NOISE_05;
    path.assign(NOISE_OUT_MIC_PATH_05s_16000);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 22050 Hz e 0.5seg\n");
    rate = MIC_RATE_NOISE_22050;
    size = MIC_LENGTH_NOISE_05;
    path.assign(NOISE_OUT_MIC_PATH_05s_22050);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    printf("Amostra ruido com amostragem 44100 Hz e 0.5seg\n");
    rate = MIC_RATE_NOISE_44100;
    size = MIC_LENGTH_NOISE_05;
    path.assign(NOISE_OUT_MIC_PATH_05s_44100);
    result = recordNoise(size, rate, path);
    if (result == ERROR) {
        exit(1);
    }

    sleep(2);

    return result;

}

int initBirdNoise() {
    int result;

    //REC a sound
    result = prepareRecordNoise();

    return result;
}

/*record
 * 
 */
int main() {
    int result;

    result = initBirdNoise();

    result = OK;
    return result;
}