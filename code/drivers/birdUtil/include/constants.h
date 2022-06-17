/* 
 * File:   constants.h
 * Author: davi
 *
 * Created on March 11, 2012, 12:08 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H


#define MIC_LENGTH 3    /* how many seconds of speech to store */
#define MIC_RATE_8000 8000   /* the sampling rate */
#define MIC_RATE_16000 16000   /* the sampling rate */
#define MIC_RATE_22050 22050   /* the sampling rate */
#define MIC_RATE_44100 44100   /* the sampling rate */
#define MIC_SIZE_8 8      /* sample size: 8 or 16 bits */
#define MIC_SIZE_16 16      /* sample size: 8 or 16 bits */
#define MIC_CHANNELS 1  /* 1 = mono 2 = stereo */
#define MIC_PATH "/dev/dsp"

#define BITS_PER_SAMPLE_16 16
#define BITS_PER_SAMPLE_8 8

#define ONE_BYTE 8

#define WORD_SIZE BITS_PER_SAMPLE_16

#define SIZE_BUFFER MIC_LENGTH*WORD_SIZE*MIC_CHANNELS/(8)

#define WAV_HEADER_LENGTH 44

//FFT Constants
#define WIN_SIZE_256 256 //fftbits
#define WIN_SIZE_512 512 //fftbits
#define WIN_SIZE_1024 1024 //fftbits
#define WIN_SIZE_2048 2048 //fftbits
#define WIN_SIZE_4096 4096 //fftbits
#define WIN_SIZE_8192 8192 //fftbits
#define WIN_SIZE_16384 16384 //fftbits
#define WIN_SIZE_32768 32768 //fftbits
#define WIN_SIZE_65536 65536 //fftbits

#define WIN_FUNCTION_BARTLETT 1
#define WIN_FUNCTION_HAMMING 2
#define WIN_FUNCTION_HANNING 3

#define WIN_FUNCTION_BARTLETT_NAME "BARTLETT"
#define WIN_FUNCTION_HAMMING_NAME "HAMMING"
#define WIN_FUNCTION_HANNING_NAME "HANNING"


//noise constants
#define MIC_LENGTH_NOISE_1 1    /* how many seconds of speech to store */
#define MIC_LENGTH_NOISE_08 0.8    /* how many seconds of speech to store */
#define MIC_LENGTH_NOISE_05 0.5    /* how many seconds of speech to store */

#define MIC_RATE_NOISE_8000 8000   /* the sampling rate */
#define MIC_RATE_NOISE_16000 16000   /* the sampling rate */
#define MIC_RATE_NOISE_22050 22050   /* the sampling rate */
#define MIC_RATE_NOISE_44100 44100   /* the sampling rate */

#define MIC_SIZE_16_NOISE 16      /* sample size: 8 or 16 bits */
#define MIC_CHANNELS_NOISE 1  /* 1 = mono 2 = stereo */

#define WAV_OUT_PATH "/home/davi/waves/out.wav"
#define WAV_OUT_MIC_PATH "/home/davi/waves/rec01.wav"

#define NOISE_OUT_MIC_PATH_1s_8000 "/home/davi/waves/noise1s_8000.wav"
#define NOISE_OUT_MIC_PATH_1s_16000 "/home/davi/waves/noise1s_16000.wav"
#define NOISE_OUT_MIC_PATH_1s_22050 "/home/davi/waves/noise1s_22050.wav"
#define NOISE_OUT_MIC_PATH_1s_44100 "/home/davi/waves/noise1s_44100.wav"

#define NOISE_OUT_MIC_PATH_05s_8000 "/home/davi/waves/noise05s_8000.wav"
#define NOISE_OUT_MIC_PATH_05s_16000 "/home/davi/waves/noise05s_16000.wav"
#define NOISE_OUT_MIC_PATH_05s_22050 "/home/davi/waves/noise05s_22050.wav"
#define NOISE_OUT_MIC_PATH_05s_44100 "/home/davi/waves/noise05s_44100.wav"

#define WAV_HEADER1 {'R', 'I', 'F', 'F'}
#define WAV_HEADER2 {'W', 'A', 'V', 'E'}
#define WAV_HEADER3 {'f', 'm', 't', 0x20}

#define WAV_CONFIG_PART1 0
#define WAV_CONFIG_PART2 1
#define WAV_CONFIG_PART3 2
#define WAV_CONFIG_PART4 3

#define BYTE_PART1 1
#define BYTE_PART2 2
#define BYTE_PART3 3
#define BYTE_PART4 4

#define FILE_CHARC_FORMAT_WEKA 0
#define FILE_CHARC_FORMAT_TXT 1

#define FILE_CHAR_PATH "/home/davi/characteristics"
#define FILE_NOISE_CHAR_PATH "/home/davi/characteristicsNoise"
#define FILE_CHAR_EXTENSION_TXT ".txt"
#define FILE_CHAR_EXTENSION_WEKA ".weka"

#define FILE_HEADER1 \
        "***********************************************\n" \
        "*                                             *\n" 
#define FILE_HEADER3 \
        "*                                             *\n" \
        "***********************************************\n"

#define FILE_HEADER4 "\n\n"

#define FILE_NAME "Nome do arquivo: "
#define FILE_RATE "Taxa de amostragem: "
#define FILE_CHANNELS "Num. de canais: "
#define FILE_TOTAL_SIZE "Tamanho total arq. WAV: "
#define FILE_DATA_SIZE "Tamanho total dados: "
#define FILE_BPS "Qtde de bits por amostra: "
#define FILE_CCODE "Código de compressão: "
#define FILE_BLOCK_ALIGN "Block align: "

#define FILE_DATE_TIME "Data/Hora da gravação: "

//TIME Domain
#define FILE_TIME_HEADER "Dados referentes ao sinal no domínio do tempo\n"
#define FILE_TIME_MEAN "Média dos dados: "
#define FILE_TIME_VARIANCE "Variância dos dados: "
#define FILE_TIME_SD "Desvio padrão dos dados: "
#define FILE_TIME_SDMEAN "Desvio médio dos dados: "
#define FILE_TIME_SKEWNESS "Assimetria (skewness) dos dados (Classif. em localização): "
#define FILE_TIME_KURTOSIS "Curtose dos dados (Classif. em variabilidade): "
#define FILE_TIME_ZCR "Taxa de passagem pelo zero (zero crossing rate) dos dados: "
#define FILE_TIME_RMSAMPLITUDE "Amplitude RMS (eficaz) dos dados: "
#define FILE_TIME_LOWESTVALUE "Menor valor dos dados: "
#define FILE_TIME_HIGHESTVALUE "Maior valor dos dados: "
#define FILE_TIME_SUM "Soma dos dados: "
#define FILE_TIME_F0 "Frequencia Fundamental dos dados: "
#define FILE_TIME_NONZEROCOUNT "Qtde de valores diferentes de zero: "

//Frequency Domain
#define FILE_FREQ_HEADER "Dados referentes ao sinal no domínio da frequencia\n"
#define FILE_FREQ_WINDOW_TYPE "Janelamento Utilizado: "
#define FILE_FREQ_WINDOW_SIZE "Tamanho da Janela: "
#define FILE_FREQ_MEAN "Média dos dados: "
#define FILE_FREQ_VARIANCE "Variância dos dados: "
#define FILE_FREQ_SD "Desvio padrão dos dados: "
#define FILE_FREQ_SDMEAN "Desvio médio dos dados: "
#define FILE_FREQ_SKEWNESS "Assimetria (skewness) dos dados (Classif. em localização): "
#define FILE_FREQ_KURTOSIS "Curtose dos dados (Classif. em variabilidade): "
#define FILE_FREQ_ROLLOF "RollOff dos dados: "
#define FILE_FREQ_CENTROID "Centróide dos dados: "
#define FILE_FREQ_SMOOTHNESS "SmoothNess dos dados: "
#define FILE_FREQ_SPREAD "Spread dos dados: "
#define FILE_FREQ_LOUDNESS "LoudNess dos dados: "
#define FILE_FREQ_FLATNESS "FlatNess dos dados: "
#define FILE_FREQ_CREST "Crista dos dados: "
#define FILE_FREQ_POWER "Potência dos dados: "


#endif	/* CONSTANTS_H */

