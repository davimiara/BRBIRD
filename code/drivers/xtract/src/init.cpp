/* libxtract feature extraction library
 *  
 * Copyright (C) 2006 Jamie Bullock
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, 
 * USA.
 */

/* init.c: defines initialisation and free functions. Also contains library constructor routine. */

#ifdef HAVE_CONFIG_H
#    include <config.h>
#endif

#include <math.h>
#include <stdlib.h>

#include "../include/libxtract.h"
#define DEFINE_GLOBALS
#include "../include/xtract_globals_private.h"

#ifdef XTRACT_FFT
#include <fftw3.h>

#ifndef XTRACT_FFT_OPTIMISATION_LEVEL
/* This should never happen */
#define XTRACT_FFT_OPTIMISATION_LEVEL 1
#endif

int xtract_init_mfcc(int N, float nyquist, int style, float freq_min, float freq_max, int freq_bands, float **fft_tables){

    int n, i, k, *fft_peak, M, next_peak; 
    float norm, mel_freq_max, mel_freq_min, norm_fact, height, inc, val, 
          freq_bw_mel, *mel_peak, *height_norm, *lin_peak;

    mel_peak = height_norm = lin_peak = NULL;
    fft_peak = NULL;
    norm = 1; 

    mel_freq_max = 1127 * log(1 + freq_max / 700);
    mel_freq_min = 1127 * log(1 + freq_min / 700);
    freq_bw_mel = (mel_freq_max - mel_freq_min) / freq_bands;

    mel_peak = (float *)malloc((freq_bands + 2) * sizeof(float)); 
    /* +2 for zeros at start and end */
    lin_peak = (float *)malloc((freq_bands + 2) * sizeof(float));
    fft_peak = (int *)malloc((freq_bands + 2) * sizeof(int));
    height_norm = (float *)malloc(freq_bands * sizeof(float));

    if(mel_peak == NULL || height_norm == NULL || 
            lin_peak == NULL || fft_peak == NULL)
        return XTRACT_MALLOC_FAILED;

    M = N >> 1;

    mel_peak[0] = mel_freq_min;
    lin_peak[0] = freq_min; // === 700 * (exp(mel_peak[0] / 1127) - 1);
    fft_peak[0] = lin_peak[0] / nyquist * M;


    for (n = 1; n < freq_bands + 2; n++){	
        //roll out peak locations - mel, linear and linear on fft window scale 
        mel_peak[n] = mel_peak[n - 1] + freq_bw_mel;
        lin_peak[n] = 700 * (exp(mel_peak[n] / 1127) -1);
        fft_peak[n] = lin_peak[n] / nyquist * M;
    }

    for (n = 0; n < freq_bands; n++){
        //roll out normalised gain of each peak
        if (style == XTRACT_EQUAL_GAIN){
            height = 1;	
            norm_fact = norm;
        }
        else{
            height = 2 / (lin_peak[n + 2] - lin_peak[n]);
            norm_fact = norm / (2 / (lin_peak[2] - lin_peak[0]));
        }
        height_norm[n] = height * norm_fact;
    }

    i = 0;

    for(n = 0; n < freq_bands; n++){

        // calculate the rise increment
        if(n==0)
            inc = height_norm[n] / fft_peak[n];
        else
            inc = height_norm[n] / (fft_peak[n] - fft_peak[n - 1]);
        val = 0;	

        // zero the start of the array
        for(k = 0; k < i; k++)
            fft_tables[n][k] = 0.f;

        // fill in the rise
        for(; i <= fft_peak[n]; i++){ 
            fft_tables[n][i] = val;
            val += inc;
        }

        // calculate the fall increment
        inc = height_norm[n] / (fft_peak[n + 1] - fft_peak[n]);

        val = 0;
        next_peak = fft_peak[n + 1];

        // reverse fill the 'fall' 
        for(i = next_peak; i > fft_peak[n]; i--){ 
            fft_tables[n][i] = val;
            val += inc;
        }

        // zero the rest of the array
        for(k = next_peak + 1; k < N; k++)
            fft_tables[n][k] = 0.f;
    }


    /* Initialise the fft_plan for the DCT */
    xtract_init_fft(freq_bands, XTRACT_MFCC);

    free(mel_peak);
    free(lin_peak);
    free(height_norm);
    free(fft_peak);

    return XTRACT_SUCCESS;

}

int xtract_init_fft(int N, int feature_name){

    float *input, *output;
    int optimisation;

    input = output = NULL;

    //fprintf(stderr, "Optimisation level: %d\n", XTRACT_FFT_OPTIMISATION_LEVEL);

    if(XTRACT_FFT_OPTIMISATION_LEVEL == 0)
        optimisation = FFTW_ESTIMATE;
    else if(XTRACT_FFT_OPTIMISATION_LEVEL == 1)
        optimisation = FFTW_MEASURE;
    else if(XTRACT_FFT_OPTIMISATION_LEVEL == 2)
        optimisation = FFTW_PATIENT;
    else
        optimisation = FFTW_MEASURE; /* The default */

    if(feature_name == XTRACT_AUTOCORRELATION_FFT)
        N <<= 1;

    input = malloc(N * sizeof(float));
    output = malloc(N * sizeof(float));

    switch(feature_name){
        case XTRACT_SPECTRUM:
            if(fft_plans.spectrum_plan != NULL)
                fftwf_destroy_plan(fft_plans.spectrum_plan);
            fft_plans.spectrum_plan = 
                fftwf_plan_r2r_1d(N, input, output, FFTW_R2HC, optimisation);
            break;
        case XTRACT_AUTOCORRELATION_FFT:
            if(fft_plans.autocorrelation_fft_plan_1 != NULL)
                fftwf_destroy_plan(fft_plans.autocorrelation_fft_plan_1);
            if(fft_plans.autocorrelation_fft_plan_2 != NULL)
                fftwf_destroy_plan(fft_plans.autocorrelation_fft_plan_2);
            fft_plans.autocorrelation_fft_plan_1 =
                fftwf_plan_r2r_1d(N, input, output, FFTW_R2HC, optimisation);
            fft_plans.autocorrelation_fft_plan_2 =
                fftwf_plan_r2r_1d(N, input, output, FFTW_HC2R, optimisation);
            break;
        case XTRACT_DCT:
            if(fft_plans.dct_plan != NULL)
                fftwf_destroy_plan(fft_plans.dct_plan);
            fft_plans.dct_plan =
                fftwf_plan_r2r_1d(N, input, output, FFTW_REDFT10, optimisation);
        case XTRACT_MFCC:
            if(fft_plans.dct_plan != NULL)
                fftwf_destroy_plan(fft_plans.dct_plan);
            fft_plans.dct_plan =
                fftwf_plan_r2r_1d(N, output, output, FFTW_REDFT00, optimisation);
            break;
    }

    free(input);
    free(output);

    return XTRACT_SUCCESS;

}

void xtract_free_fft(void){
    if(fft_plans.spectrum_plan != NULL)
        fftwf_destroy_plan(fft_plans.spectrum_plan);
    if(fft_plans.autocorrelation_fft_plan_1 != NULL)
        fftwf_destroy_plan(fft_plans.autocorrelation_fft_plan_1);
    if(fft_plans.autocorrelation_fft_plan_2 != NULL)
        fftwf_destroy_plan(fft_plans.autocorrelation_fft_plan_2);
    if(fft_plans.dct_plan != NULL)
        fftwf_destroy_plan(fft_plans.dct_plan);
//    fftwf_cleanup();
}

#endif

int xtract_init_bark(int N, float sr, int *band_limits){

    float  edges[] = {0, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500, 20500, 27000}; /* Takes us up to sr = 54kHz (CCRMA: JOS)*/

    int bands = XTRACT_BARK_BANDS;

    while(bands--)
        band_limits[bands] = edges[bands] / sr * N;
    /*FIX shohuld use rounding, but couldn't get it to work */

    return XTRACT_SUCCESS;
}

void xtract_free_window(float *window){

    free(window);

}

#ifdef __GNUC__
__attribute__((constructor)) void init()
#else
    void _init()·
#endif
{
#ifdef XTRACT_FFT
    fft_plans.spectrum_plan = NULL;
    fft_plans.autocorrelation_fft_plan_1 = NULL;
    fft_plans.autocorrelation_fft_plan_2 = NULL;
    fft_plans.dct_plan = NULL;
#endif
}
