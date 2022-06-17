/*
 * 		MESTRADO COMPUTAÇÃO APLICADA
 *
 *   Classe responsável pela montagem e extração de características do som 
 * gravado.
 *   Funções:
 *  - Criação arquivo texto (padrão WEKA)
 *  - Chamada da biblioteca de extração de características
 *      - Caracterísiticas em função do tempo (sem FFTW)
 *  - Chamada da função FFT para extração de outras carac. do arquivo
 *      - Amostragem
 *      - Canais
 *      - Bytes
 *      - Frequencias
 *      - Janelamento (tipo de função)
 *  - Características salvas no linux
 *      - Data Hora
 *      - Localização
 * Autor: Davi Miara Kiapuchinski
 * Data: 10/03/2012
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

#include "../include/charExtractor.h"
#include <bird/constants.h>
#include <bird/libxtract.h>
#include <bird/xtract_scalar.h>

/*----------------------------------===-----------------------------------------
 *
 *                             Construtor
 *
 *----------------------------------===---------------------------------------*/

charExtractor::charExtractor() {
}

charExtractor::~charExtractor() {
}



/*----------------------------------===-----------------------------------------
 *
 *                             Métodos
 *
 *----------------------------------===---------------------------------------*/

/**
 * Cria arquivo de características pertinentes de um arquivo WAV
 * @param path - caminho do arquivo wave
 * @param value - valor do parametro recebido
 * @param destPaht - Caminho do arquivo de saida
 * @return OK or ERROR
 */
int charExtractor::createCharFile(int format, void * obj,
        string destPath) {
    int result = ERROR;

    stringstream filename;
    if (!strcmp(destPath.c_str(), "")) filename << FILE_CHAR_PATH;
    else filename << destPath.c_str();
    filename << ((format == FILE_CHARC_FORMAT_WEKA) ? FILE_CHAR_EXTENSION_WEKA
            : FILE_CHAR_EXTENSION_TXT);

    InitFFT* fileWav = (InitFFT*) obj;

    //write header
    stringstream content;
    content << FILE_HEADER1;
    content << "\t" << filename.str() << "\n";
    content << FILE_HEADER3;
    content << FILE_HEADER4;
    content << FILE_NAME << fileWav->getFileToFFT().c_str() << "\n";

    //call wave char
    result = openWave(fileWav);
    if (result == OK) {
        //take values from file

        //take data
        setDataSize(fileWav->getDataSize());
        setDataTime(fileWav->getDataWav());

        //take total size
        content << FILE_TOTAL_SIZE << fileWav->getTotalSize() << "\n";
        //take data size
        content << FILE_DATA_SIZE << fileWav->getDataSize() << "\n";
        //take rate
        content << FILE_RATE << fileWav->getRate() << "\n";
        //take channels
        content << FILE_CHANNELS << fileWav->getChannels() << "\n";
        //compression code
        content << FILE_CCODE << fileWav->getCompressionCode() << "\n";
        //bits per sample
        content << FILE_BPS << fileWav->getBps() << "\n";
        //block Align
        content << FILE_BLOCK_ALIGN << fileWav->getBlockAlign() << "\n\n";

        time_t lt = time(NULL);
        content << FILE_DATE_TIME << asctime(localtime(&lt)) << "\n";

        //call timeChar

        clock_t inicio, fim;
        inicio = clock();

        result = timeChar(fileWav);

        fim = clock();

        //printf("Characteristics Time Domain - O tempo de execucao em milisegundos é %lf\n", (double) (fim - inicio) / 1000);

        //CALL frequency char
        if (result == OK) {

            printf("Characteristics Time Domain - O tempo de execucao em segundos é %lf\n", (double) (fim - inicio) / CLOCKS_PER_SEC);

            //calculate fft
            result = fileWav->fastFourier();
            if (result == OK) {

                setDataFreq(fileWav->getFreqDataWav());
                setDataSizeFreq(fileWav->getDataFreqSize());

                inicio = 0;
                fim = 0;
                inicio = clock();
                result = freqChar(fileWav);
                fim = clock();
            }

        }

        if (result != ERROR) {

            printf("Characteristics Frequency Domain - O tempo de execucao em segundos é %lf\n", (double) (fim - inicio) / CLOCKS_PER_SEC);

            content << FILE_HEADER1;
            content << FILE_TIME_HEADER;
            content << FILE_HEADER3;
            content << FILE_TIME_MEAN << getDataTimeMean() << "\n";
            content << FILE_TIME_VARIANCE << getDataTimeVariance() << "\n";
            content << FILE_TIME_SD << getDataTimeSD() << "\n";
            content << FILE_TIME_SDMEAN << getDataTimeSDMean() << "\n";
            content << FILE_TIME_SKEWNESS << getDataTimeSkewness() << "\n";
            content << FILE_TIME_KURTOSIS << getDataTimeKurtosis() << "\n";
            content << FILE_TIME_ZCR << getDataTimeZCR() << "\n";
            content << FILE_TIME_RMSAMPLITUDE << getDataTimeRMSAmplitude() << "\n";
            content << FILE_TIME_LOWESTVALUE << getDataTimeLowestValue() << "\n";
            content << FILE_TIME_HIGHESTVALUE << getDataTimeHighestValue() << "\n";
            content << FILE_TIME_SUM << getDataTimeSum() << "\n";
            //content << FILE_TIME_F0 << getDataTimeF0() << "\n";
            content << FILE_TIME_NONZEROCOUNT << getDataTimeNonZeroCount() << "\n";

            //frequency char
            content << "\n\n";
            content << FILE_HEADER1;
            content << FILE_FREQ_HEADER;
            content << FILE_HEADER3;


            content << FILE_FREQ_WINDOW_TYPE << getWindowName(fileWav->getWindowType()) << "\n";
            content << FILE_FREQ_WINDOW_SIZE << fileWav->getWinSize() << "\n";
            content << FILE_FREQ_CENTROID << getDataFreqMean() << "\n";
            content << FILE_FREQ_VARIANCE << getDataFreqVariance() << "\n";
            content << FILE_FREQ_SD << getDataFreqSD() << "\n";
            content << FILE_FREQ_SDMEAN << getDataFreqSDMean() << "\n";
            content << FILE_FREQ_SKEWNESS << getDataFreqSkewness() << "\n";
            content << FILE_FREQ_KURTOSIS << getDataFreqKurtosis() << "\n";
            content << FILE_FREQ_ROLLOF << getDataFreqRollOff() << "\n";
            content << FILE_FREQ_CENTROID << getDataFreqCentroid() << "\n";
            content << FILE_FREQ_SMOOTHNESS << getDataFreqSmoothness() << "\n";
            content << FILE_FREQ_SPREAD << getDataFreqSpread() << "\n";
            //content << FILE_FREQ_LOUDNESS << getDataFreqLoudness() << "\n";
            //content << FILE_FREQ_FLATNESS << getDataFreqFlatness() << "\n";
            content << FILE_FREQ_CREST << getDataFreqCrest() << "\n";
            //content << FILE_FREQ_POWER << getDataFreqPower() << "\n";

            //call ambianceChar


        } else result = ERROR;

    } else {
    }

    if (result == OK) {
        //CREATE FILE
        file = fopen(filename.str().c_str(), "w"); //create a file
        //write on file
        fputs(content.str().c_str(), file);
        //close file
        fclose(file);
    } else {

    }

    return result;
}

/**
 * Extrai características de um arquivo WAVE
 * @param parameters - parametro recebido
 * @param value - valor do parametro recebido
 * @param tablePosition - valor do indice da tabela recebido
 * @return OK or ERROR
 */
int charExtractor::waveChar(void * obj) {
    int result = ERROR;
    //call openWave
    result = openWave(obj);
    return result;
}

/**
 * Extrai características de frequencia em um arquivo WAVE
 * @param parameters - parametro recebido
 * @param value - valor do parametro recebido
 * @param tablePosition - valor do indice da tabela recebido
 * @return OK or ERROR
 */
int charExtractor::freqChar(void * obj) {
    int result = ERROR;

    //call libxtract
    result = xtract_spectral_mean(getDataFreq(), getDataSizeFreq(), NULL,
            &dataFreqMean);
    if (result == XTRACT_SUCCESS) {
//        printf("Calculando Freq. média\n");
        setDataFreqMean(dataFreqMean);
        result = OK;
    } else {
        result = ERROR;
    }

    //VARIANCE

    float aux = getDataFreqMean();
    if (result != ERROR && (xtract_spectral_variance(getDataFreq(),
            getDataSizeFreq() / 2, &aux, &dataFreqVariance) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. Variance\n");
        setDataFreqVariance(dataFreqVariance);
    } else result = ERROR;

    //standart deviation
    //    printf("Calculando Freq. SD\n");
    setDataFreqSD(sqrt(getDataFreqVariance()));

    //average deviation

    aux = getDataFreqMean();
    if (result != ERROR && (xtract_spectral_average_deviation(getDataFreq(),
            getDataSizeFreq(), &aux, &dataFreqSDMean) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. SD_avg\n");
        setDataFreqSDMean(dataFreqSDMean);
    } else result = ERROR;

    //skewness

    float aux2[2] = {getDataFreqMean(), getDataFreqSD()};
    if (result != ERROR && (xtract_spectral_average_deviation(getDataFreq(),
            getDataSizeFreq(), &aux2, &dataFreqSkewness) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. Skewness\n");
        setDataFreqSkewness(dataFreqSkewness);
    } else result = ERROR;

    //kurtosis
    if (result != ERROR && (xtract_spectral_kurtosis(getDataFreq(),
            getDataSizeFreq(), &aux2, &dataFreqKurtosis) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. kurtosis\n");
        setDataFreqKurtosis(dataFreqKurtosis);
    } else result = ERROR;

    //RollOff
    InitFFT* fileWav = (InitFFT*) obj;
    int rate = fileWav->getRate();
    aux2[0] = (float) rate / getDataSizeFreq();
    aux2[1] = 85;
    if (result != ERROR && (xtract_rolloff(getDataFreq(),
            getDataSizeFreq(), &aux2, &dataFreqRollOff) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. RollOff\n");
        setDataFreqRollOff(dataFreqRollOff);
    } else result = ERROR;

    //Centroid

    if (result != ERROR && (xtract_spectral_centroid(getDataFreq(),
            getDataSizeFreq(), NULL, &dataFreqCentroid) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. Centroid\n");
        setDataFreqCentroid(dataFreqCentroid);
    } else result = ERROR;

    //Smoothness

    if (result != ERROR && (xtract_smoothness(getDataFreq(),
            getDataSizeFreq(), NULL, &dataFreqSmoothness) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. Smootness\n");
        setDataFreqSmoothness(dataFreqSmoothness);
    } else result = ERROR;

    //Spread

    if (result != ERROR && (xtract_spread(getDataFreq(), getDataSizeFreq(),
            &dataFreqCentroid, &dataFreqSpread) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. Spread\n");
        setDataFreqSmoothness(dataFreqSpread);
    } else result = ERROR;

    //Loundness
    //    printf("Calculando Freq. loudness\n");
    //    int * band_limits = 0;
    //    float * resultBark = 0;
    //    //init Vector
    //    result = xtract_init_bark(fileWav->getWinSize(), fileWav->getRate(),
    //            band_limits);
    //    //Get Vector
    //    result = xtract_bark_coefficients(getDataFreq(), getDataSizeFreq(),
    //            band_limits, resultBark);
    //    if (result != ERROR && (xtract_loudness(resultBark,
    //            XTRACT_BARK_BANDS, NULL, &dataFreqLoudness) == XTRACT_SUCCESS))
    //        setDataFreqLoudness(dataFreqLoudness);
    //    else result = ERROR;

//    //flatness
//    if (result != ERROR && (xtract_flatness(getDataFreq(), getDataSizeFreq(), NULL,
//            &dataFreqFlatness) == XTRACT_SUCCESS)) {
//        printf("Calculando Freq. Flatness\n");
//        setDataFreqFlatness(dataFreqFlatness);
//    } else result = ERROR;

    //crest
    //xtract_highest_value
    if (result != ERROR && (xtract_highest_value(getDataFreq(),
            getDataSizeFreq(), NULL, &dataTimeHighestValue) == XTRACT_SUCCESS))
        setDataTimeHighestValue(dataTimeHighestValue);
    else result = ERROR;
    aux2[0] = getDataTimeHighestValue(); //max value 
    aux2[1] = getDataFreqMean(); //mean value
    if (result != ERROR && (xtract_crest(NULL, 0, aux2, &dataFreqCrest)
            == XTRACT_SUCCESS)) {
        setDataFreqFlatness(dataFreqCrest);
//        printf("Calculando Freq. Crest\n");
    } else result = ERROR;


    return result;
}

/**
 * Extrai características de tempo em um arquivo WAVE
 * @param parameters - parametro recebido
 * @param value - valor do parametro recebido
 * @param tablePosition - valor do indice da tabela recebido
 * @return OK or ERROR
 */
int charExtractor::timeChar(void * obj) {
    int result = ERROR;

    //InitFFT* fileWav = (InitFFT*) obj;

    //call libxtract
    result = xtract_mean(getDataTime(), getDataSize(), NULL, &dataTimeMean);
    if (result == XTRACT_SUCCESS) {
        setDataTimeMean(dataTimeMean);
        result = OK;
    } else {
        result = ERROR;
    }

    //VARIANCE
    float aux = getDataTimeMean();
    if (result != ERROR && (xtract_variance(getDataTime(), getDataSize(),
            &aux, &dataTimeVariance) == XTRACT_SUCCESS))
        setDataTimeVariance(dataTimeVariance);
    else result = ERROR;

    //standart deviation
    aux = getDataTimeVariance();
    if (result != ERROR && (xtract_standard_deviation(getDataTime(),
            getDataSize(), &aux, &dataTimeSD) == XTRACT_SUCCESS))
        setDataTimeSD(dataTimeSD);
    else result = ERROR;

    //average deviation
    aux = getDataTimeMean();
    if (result != ERROR && (xtract_average_deviation(getDataTime(),
            getDataSize(), &aux, &dataTimeSDMean) == XTRACT_SUCCESS))
        setDataTimeSDMean(dataTimeSDMean);
    else result = ERROR;

    //skewness
    float aux2[2] = {getDataTimeMean(), getDataTimeSD()};
    if (result != ERROR && (xtract_skewness(getDataTime(),
            getDataSize(), &aux2, &dataTimeSkewness) == XTRACT_SUCCESS))
        setDataTimeSkewness(dataTimeSkewness);
    else result = ERROR;

    //kurtosis
    if (result != ERROR && (xtract_kurtosis(getDataTime(),
            getDataSize(), &aux2, &dataTimeKurtosis) == XTRACT_SUCCESS))
        setDataTimeKurtosis(dataTimeKurtosis);
    else result = ERROR;

    //xtract_zcr
    if (result != ERROR && (xtract_zcr(getDataTime(),
            getDataSize(), NULL, &dataTimeZCR) == XTRACT_SUCCESS))
        setDataTimeZCR(dataTimeZCR);
    else result = ERROR;

    //xtract_rms_amplitude
    if (result != ERROR && (xtract_rms_amplitude(getDataTime(),
            getDataSize(), NULL, &dataTimeRMSAmplitude) == XTRACT_SUCCESS))
        setDataTimeRMSAmplitude(dataTimeRMSAmplitude);
    else result = ERROR;

    //xtract_lowest_value
    aux = -0.99999;
    if (result != ERROR && (xtract_lowest_value(getDataTime(),
            getDataSize(), &aux, &dataTimeLowestValue) == XTRACT_SUCCESS))
        setDataTimeLowestValue(dataTimeLowestValue);
    else result = ERROR;

    //xtract_highest_value
    if (result != ERROR && (xtract_highest_value(getDataTime(),
            getDataSize(), NULL, &dataTimeHighestValue) == XTRACT_SUCCESS))
        setDataTimeHighestValue(dataTimeHighestValue);
    else result = ERROR;

    //xtract_sum
    if (result != ERROR && (xtract_sum(getDataTime(),
            getDataSize(), NULL, &dataTimeSum) == XTRACT_SUCCESS))
        setDataTimeSum(dataTimeSum);
    else result = ERROR;

    //xtract_f0
    //    aux = (float) fileWav->getRate();
    //    if (result != ERROR && (xtract_f0(getDataTime(),
    //            getDataSize(), &aux, &dataTimeF0) == XTRACT_SUCCESS))
    //        setDataTimeF0(dataTimeF0);
    //    else result = ERROR;

    //xtract_nonzero_count
    if (result != ERROR && (xtract_nonzero_count(getDataTime(),
            getDataSize(), NULL, &dataTimeNonZeroCount) == XTRACT_SUCCESS))
        setDataTimeNonZeroCount(dataTimeNonZeroCount);
    else result = ERROR;


    return result;
}

/**
 * Abre e pega dados de um arquivo WAVE
 * @param path - Caminho arquivo wave
 * @param data - Dados Arquivo WAV
 * @param rate - Taxa de amostragem
 * @param channels - Número de canais
 * @param totalSize - Tamanho total arquivo wave
 * @return OK or ERROR
 */
int charExtractor::openWave(void * obj) {
    int result = ERROR;
    //call init FFT
    InitFFT * fileWav = (InitFFT*) obj;
    result = fileWav->openWave();

    return result;
}

void charExtractor::setLocal(string local) {

    this->local = local;
}

string charExtractor::getLocal() const {

    return local;
}

void charExtractor::setDatetime(string datetime) {

    this->datetime = datetime;
}

string charExtractor::getDatetime() const {

    return datetime;
}

void charExtractor::setWindowing(string windowing) {

    this->windowing = windowing;
}

string charExtractor::getWindowing() const {

    return windowing;
}

void charExtractor::setDataSizeFreq(int dataSizeFreq) {
    this->dataSizeFreq = dataSizeFreq;
}

int charExtractor::getDataSizeFreq() {
    return dataSizeFreq;
}

void charExtractor::setDataFreq(float* dataFreq) {

    this->dataFreq = dataFreq;
}

float* charExtractor::getDataFreq() {

    return dataFreq;
}

void charExtractor::setDataTime(float* dataTime) {

    this->dataTime = dataTime;
}

float* charExtractor::getDataTime() const {

    return dataTime;
}

void charExtractor::setDataSize(int dataSize) {

    this->dataSize = dataSize;
}

int charExtractor::getDataSize() {

    return dataSize;
}

void charExtractor::setDataTimeNonZeroCount(float dataTimeNonZeroCount) {

    this->dataTimeNonZeroCount = dataTimeNonZeroCount;
}

float charExtractor::getDataTimeNonZeroCount() const {

    return dataTimeNonZeroCount;
}

void charExtractor::setDataTimeF0(float dataTimeF0) {

    this->dataTimeF0 = dataTimeF0;
}

float charExtractor::getDataTimeF0() const {

    return dataTimeF0;
}

void charExtractor::setDataTimeSum(float dataTimeSum) {

    this->dataTimeSum = dataTimeSum;
}

float charExtractor::getDataTimeSum() const {

    return dataTimeSum;
}

void charExtractor::setDataTimeHighestValue(float dataTimeHighestValue) {

    this->dataTimeHighestValue = dataTimeHighestValue;
}

float charExtractor::getDataTimeHighestValue() const {

    return dataTimeHighestValue;
}

void charExtractor::setDataTimeLowestValue(float dataTimeLowestValue) {

    this->dataTimeLowestValue = dataTimeLowestValue;
}

float charExtractor::getDataTimeLowestValue() const {

    return dataTimeLowestValue;
}

void charExtractor::setDataTimeRMSAmplitude(float dataTimeRMSAmplitude) {

    this->dataTimeRMSAmplitude = dataTimeRMSAmplitude;
}

float charExtractor::getDataTimeRMSAmplitude() const {

    return dataTimeRMSAmplitude;
}

void charExtractor::setDataTimeKurtosis(float dataTimeKurtosis) {

    this->dataTimeKurtosis = dataTimeKurtosis;
}

float charExtractor::getDataTimeKurtosis() const {

    return dataTimeKurtosis;
}

void charExtractor::setDataTimeSkewness(float dataTimeSkewness) {

    this->dataTimeSkewness = dataTimeSkewness;
}

float charExtractor::getDataTimeSkewness() const {

    return dataTimeSkewness;
}

void charExtractor::setDataTimeZCR(float dataFreqZCR) {

    this->dataTimeZCR = dataFreqZCR;
}

float charExtractor::getDataTimeZCR() const {

    return dataTimeZCR;
}

void charExtractor::setDataTimeSDMean(float dataTimeSDMean) {

    this->dataTimeSDMean = dataTimeSDMean;
}

float charExtractor::getDataTimeSDMean() const {

    return dataTimeSDMean;
}

void charExtractor::setDataTimeSD(float dataTimeSD) {

    this->dataTimeSD = dataTimeSD;
}

float charExtractor::getDataTimeSD() const {

    return dataTimeSD;
}

void charExtractor::setDataTimeVariance(float dataTimeVariance) {

    this->dataTimeVariance = dataTimeVariance;
}

float charExtractor::getDataTimeVariance() const {

    return dataTimeVariance;
}

void charExtractor::setDataTimeMean(float dataTimeMean) {

    this->dataTimeMean = dataTimeMean;
}

float charExtractor::getDataTimeMean() {

    return dataTimeMean;
}

void charExtractor::setDataFreqLoudness(float dataFreqLoudness) {

    this->dataFreqLoudness = dataFreqLoudness;
}

float charExtractor::getDataFreqLoudness() const {

    return dataFreqLoudness;
}

void charExtractor::setDataFreqPower(float dataFreqPower) {

    this->dataFreqPower = dataFreqPower;
}

float charExtractor::getDataFreqPower() const {

    return dataFreqPower;
}

void charExtractor::setDataFreqCrest(float dataFreqCrest) {

    this->dataFreqCrest = dataFreqCrest;
}

float charExtractor::getDataFreqCrest() const {

    return dataFreqCrest;
}

void charExtractor::setDataFreqFlatness(float dataFreqFlatness) {

    this->dataFreqFlatness = dataFreqFlatness;
}

float charExtractor::getDataFreqFlatness() const {

    return dataFreqFlatness;
}

void charExtractor::setDataFreqRollOff(float dataFreqRollOff) {

    this->dataFreqRollOff = dataFreqRollOff;
}

float charExtractor::getDataFreqRollOff() const {

    return dataFreqRollOff;
}

void charExtractor::setDataFreqSpread(float dataFreqSpread) {

    this->dataFreqSpread = dataFreqSpread;
}

float charExtractor::getDataFreqSpread() const {

    return dataFreqSpread;
}

void charExtractor::setDataFreqSmoothness(float dataFreqSmoothness) {

    this->dataFreqSmoothness = dataFreqSmoothness;
}

float charExtractor::getDataFreqSmoothness() const {

    return dataFreqSmoothness;
}

void charExtractor::setDataFreqSkewness(float dataFreqSkewness) {

    this->dataFreqSkewness = dataFreqSkewness;
}

float charExtractor::getDataFreqSkewness() const {

    return dataFreqSkewness;
}

void charExtractor::setDataFreqCentroid(float dataFreqCentroid) {

    this->dataFreqCentroid = dataFreqCentroid;
}

float charExtractor::getDataFreqCentroid() const {

    return dataFreqCentroid;
}

void charExtractor::setDataFreqKurtosis(float dataFreqKurtosis) {

    this->dataFreqKurtosis = dataFreqKurtosis;
}

float charExtractor::getDataFreqKurtosis() const {

    return dataFreqKurtosis;
}

void charExtractor::setDataFreqSDMean(float dataFreqSDMean) {

    this->dataFreqSDMean = dataFreqSDMean;
}

float charExtractor::getDataFreqSDMean() const {

    return dataFreqSDMean;
}

void charExtractor::setDataFreqSD(float dataFreqSD) {

    this->dataFreqSD = dataFreqSD;
}

float charExtractor::getDataFreqSD() const {

    return dataFreqSD;
}

void charExtractor::setDataFreqVariance(float dataFreqVariance) {

    this->dataFreqVariance = dataFreqVariance;
}

float charExtractor::getDataFreqVariance() const {

    return dataFreqVariance;
}

void charExtractor::setDataFreqMean(float dataFreqMean) {

    this->dataFreqMean = dataFreqMean;
}

float charExtractor::getDataFreqMean() const {
    return dataFreqMean;
}