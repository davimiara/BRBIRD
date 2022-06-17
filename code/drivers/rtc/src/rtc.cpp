/*
 *		HENRY EQUIPAMENTOS ELETRÔNICOS E SISTEMAS LTDA
 *
 *   Classe responsável por gerenciar a comunicação com o módulo RTC.
 *
 * Autor: Felipe dos Santos
 * Data: 04/2010
 *
 * Modificações:
 *
 * ------------------------------------===--------------------------------------
 * Autor: Eduardo de Andrade
 * Data : 29/06/2010
 * Alteração : Métodos refatorados. Criação dos métodos de conexão e desconexão.
 * ------------------------------------===--------------------------------------
 * Autor: Felipe dos Santos
 * Data : 16/07/2010
 * Alteração : Implementação de controle de horário de verão.
 * Implementação de evento para disparar quando a data/hora é alterada.
 * ------------------------------------===--------------------------------------
 *
 */

#include <stdio.h>
#include <string.h>
#include "../include/rtc.h"

#define OK 0
#define ERROR -1

#define HRTC_DEBUG "HRTC_DEBUG"


#define RTC_SEC 0x00
#define RTC_MIN 0x01
#define RTC_HOUR 0x02
#define RTC_WDAY 0x03
#define RTC_MDAY 0x04
#define RTC_MON 0x05
#define RTC_YEAR 0x06

#define RTC_ADDRESS 0x68

#define RTC_AGGING_OFFSET 0x10

#define RTC_COMMAND_REGISTER 0x0E
//-- bits do registrador de comando
#define RTC_BIT_CONVERT_TEMPERATURE 0x20 //bit 5

#define RTC_STATUS_REGISTER 0x0F
//-- bits do registrador de status
#define RTC_BIT_BUSY 0x04 //bit 2


/*
 * Pìnos de controle RTC
 *
 * - CON 6
 * SCL - 19
 * SDA - 20
 *
 */

HRtc* hrtc;

/**
 * Construtor da classe HRtc.
 */
HRtc::HRtc() {
    initialize();
    strcpy(this->device, "");
}

/**
 * Construtor da classe HRtc.
 *
 * @param *address Endereço do dispositivo RTC.
 */
HRtc::HRtc(char* address) {
    initialize();
    strcpy(this->device, address);
}

/**
 * Construtor da classe HRtc.
 *
 * @param initSummerTime Estrutura tm com a data de inicio do horario de verão
 * (dia mes e ano apenas).
 * @pram endSummerTime Estrutura tm com a data de término do horario de verão
 * (dia mes e ano apenas).
 */
HRtc::HRtc(tm initSummerTime, tm endSummerTime) {
    initialize();
    this->adjustSummerTime(initSummerTime, endSummerTime);
}

/**
 * Construtor da classe HRtc.
 *
 * @param *address Endereço do dispositivo RTC.
 * @param initSummerTime Estrutura tm com a data de inicio do horario de verão
 * (dia mes e ano apenas).
 * @pram endSummerTime Estrutura tm com a data de término do horario de verão
 * (dia mes e ano apenas).
 */
HRtc::HRtc(char* address, tm initSummerTime, tm endSummerTime) {
    initialize();
    strcpy(this->device, address);
    this->adjustSummerTime(initSummerTime, endSummerTime);
}

/**
 * Destrutor da classe Hic2.
 */
HRtc::~HRtc() {
    disconnect(); //desconectando do socket
}

/*-------------------------------------===--------------------------------------
 *
 *                              MÉTODOS PRIVADOS
 *
 *-------------------------------------===------------------------------------*/

/**
 * Inicializa as variáveis da classe.
 *
 * @return 0.
 */
int HRtc::initialize() {
    this->debugLevel = getEnvValue((char*) HRTC_DEBUG);
    this->terminated = false;
    this->paused = false;
    this->errorFlag = false;
    this->summer = false;
    this->unSummer = false;
    this->running = false;
    this->object = NULL;
    this->changedDateTimeEventHandler = NULL;

    memset(&rtcTime, 0, sizeof (tm));
    memset(&initSummerTime, 0, sizeof (tm));
    memset(&endSummerTime, 0, sizeof (tm));

    this->initSummerTime.tm_hour = 0;
    this->initSummerTime.tm_min = 0;

    this->endSummerTime.tm_hour = 23;
    this->endSummerTime.tm_min = 59;
    return OK;
}

/**
 * Envia os dados de configuração de data para o RTC.
 *
 * @param adress Endereço do RTC para configurar o tipo de data desejada.
 * @param value Data a ser configurada.
 * @return 0 para OK, -1 para ERRO. *
 */
int HRtc::sendData(int adress, unsigned int value) {
    try {
        int result = ds3231_write_byte(&rtc, adress, value);
        if (result < 0)
            errorFlag = true;
        else
            errorFlag = false;
        if (this->debugLevel >= H_DBG_LVL_5) {
            char str[64] = "";
            sprintf(str, "Endereço 0x%02X - Valor %d", adress, value);
            debug(str, (result >= OK) ? "OK" : "ERROR");
        }
        return result;
    } catch (...) {

    }
    return ERROR;
}

/**
 * Lê o módulo RTC e retorna o valor (Ex: hora) requisitado.
 *
 * @param adress Endereço do parametro no módulo RTC que se deseja ler.
 * @return Valor do lido do RTC.
 */
int HRtc::receiveData(int adress) {
    try {
        int result = ds3231_read_byte(&rtc, adress);
        errorFlag = (result < 0) ? true : false;
        if (this->debugLevel >= H_DBG_LVL_5) {
            char str[64] = "";
            sprintf(str, "Endereço 0x%02X - Valor %d", adress, result);
            debug(str, "");
        }
        return result;
    } catch (...) {

    }
    return ERROR;
}

/**
 * Verifica se o módulo RTC está ocupado.
 *
 * @return True ou False.
 */
bool HRtc::busy() {
    try {
        return (this->receiveData(RTC_STATUS_REGISTER) & RTC_BIT_BUSY) ?
                true : false;
    } catch (...) {

    }
    return ERROR;
}

/**
 * Ativa a compensação de temperatura automática do módulo RTC.
 *
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::temperatureCompensation() {
    try {
        int wait = 0;
        while (this->busy()) { //verifica se rtc está ocupado
            if (wait == 3)
                return ERROR;
            usleep(500000); //espera 500ms
            wait++;
        }
        //Recebendo dados do registrador
        int bits = this->receiveData(RTC_COMMAND_REGISTER);
        if (bits < 0) //verificando valor recebido
            return ERROR;
        bits |= RTC_BIT_CONVERT_TEMPERATURE; //setando comando
        return (ds3231_write_byte(&rtc, 0x0E, bits) >= 0) ? OK : ERROR;
    } catch (...) {

    }
    return ERROR;
}

/**
 * Thread de envio de comando de compensação de temperatura. É executada a cada
 * 5 segundos.
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::exec() {
    int result = ERROR;
    try {
        while (!terminated) { //condição de parada
            for (int i = 0; i < 10; i++) { //10 sleeps de 500 ms = 5s
                if (terminated) break;
                if (!paused)
                    this->getDateTime();
                usleep(500000); //sleep 500 ms
            }
            while (paused) usleep(500000);
            this->temperatureCompensation(); //ajustando rtc
            usleep(10);
            this->setSummerTime();
            this->unSetSummerTime();
        }
        result = OK;
    } catch (...) {

    }
    this->running = false;
    this->detach();
    return result;
}

/**
 * Lê a data e hora do módulo RTC.
 * @param time Ponteiro para estrutura contendo os dados referentes a data e
 * hora a serem lidos.
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::getDateTime() {
    try {
        int addr = RTC_SEC; //Recebendo dados do RTC no endereço 0
        int bcd = this->receiveData(addr++); //segundos
        rtcTime.tm_sec = BCD_TO_BIN(bcd);
        bcd = this->receiveData(addr++); //minutos
        rtcTime.tm_min = BCD_TO_BIN(bcd);
        bcd = this->receiveData(addr++); //horas
        rtcTime.tm_hour = BCD_TO_BIN(bcd);
        bcd = this->receiveData(addr++); //dia da semana
        rtcTime.tm_wday = BCD_TO_BIN(bcd);
        bcd = this->receiveData(addr++); //dia do mês
        rtcTime.tm_mday = BCD_TO_BIN(bcd);
        bcd = this->receiveData(addr++); //mês
        rtcTime.tm_mon = BCD_TO_BIN(bcd) - 1;
        bcd = this->receiveData(addr++); //ano
        rtcTime.tm_year = BCD_TO_BIN(bcd) + 100;
        return OK;
    } catch (...) {

    }
    return ERROR;
}

/**
 * Configura o horario de verão.
 *
 * @return 0.
 */
int HRtc::setSummerTime() {
    if ((summer) && (summerTimeEnable)) {
        if (this->initSummerTime.tm_year > rtcTime.tm_year)
            return OK;
        if (this->initSummerTime.tm_mon > rtcTime.tm_mon)
            return OK;
        if (this->initSummerTime.tm_mday > rtcTime.tm_mday)
            return OK;
        if (this->initSummerTime.tm_hour > rtcTime.tm_hour)
            return OK;
        if (this->initSummerTime.tm_min > rtcTime.tm_min)
            return OK;
        tm temp = rtcTime;
        temp.tm_hour++;
        this->setDateTime(&temp);
        printf("Ajustou horario de verao!\n");
        summer = false;
        unSummer = true;
    }
    return OK;
}

/**
 * Configura o horario "normal".
 *
 * @return 0.
 */
int HRtc::unSetSummerTime() {
    if ((unSummer) && (summerTimeEnable)) {
        if (this->endSummerTime.tm_year > rtcTime.tm_year)
            return OK;
        if (this->endSummerTime.tm_mon > rtcTime.tm_mon)
            return OK;
        if (this->endSummerTime.tm_mday > rtcTime.tm_mday)
            return OK;
        if (this->endSummerTime.tm_hour > rtcTime.tm_hour)
            return OK;
        if (this->endSummerTime.tm_min > rtcTime.tm_min)
            return OK;
        tm temp = rtcTime;
        temp.tm_hour--;
        this->setDateTime(&temp);
        unSummer = false;
    }
    return OK;
}

/*-------------------------------------===--------------------------------------
 *
 *                              MÉTODOS PÚBLICOS
 *
 *-------------------------------------===------------------------------------*/

/**
 * Conecta no endereço fornecido no construtor.
 *
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::connect() {
    return this->connect(this->device);
}

/**
 * Conecta no endereço informado no parâmetro.
 *
 * @param address Endereço da conexão (Ex: "/dev/i2c/0").
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::connect(char* address) {
    if (strcmp(this->device, "") == 0)
        return ERROR;
    strcpy(this->device, address);
    if (!this->isRunning()) {
        if (ds3231_open(address, RTC_ADDRESS, &rtc) == OK) {
            //printf("Thread Inicializada\n");
            this->terminated = false;
            this->start();
            this->running = true;
        } else {
            return ERROR;
        }
    }
    return OK;
}

/**
 * Fecha socket de comunicação.
 *
 * @return 0.
 */
int HRtc::disconnect() {
    this->pause(false); //despausando thread
    this->terminate(); //parando thread
    ds3231_close(&rtc); //fechando socket I2C
    return OK;
}

/**
 *  Configura o horario de verão.
 *
 * @param initSummerTime Estrutura tm com a data de inicio do horario de verão
 * (dia mes e ano apenas).
 * @pram endSummerTime Estrutura tm com a data de término do horario de verão
 * (dia mes e ano apenas).
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::adjustSummerTime(tm initSummerTime, tm endSummerTime) {
    try {
        printf("horario de verao setado\n");
        summerTimeEnable = true;
        summer = true;
        this->initSummerTime.tm_mday = initSummerTime.tm_mday;
        this->initSummerTime.tm_mon = initSummerTime.tm_mon;
        this->initSummerTime.tm_year = initSummerTime.tm_year;

        this->endSummerTime.tm_mday = endSummerTime.tm_mday;
        this->endSummerTime.tm_mon = endSummerTime.tm_mon;
        this->endSummerTime.tm_year = endSummerTime.tm_year;
        return OK;
    } catch (...) {

    }
    return ERROR;
}

/**
 *  Extrai o horario de verão configurado.
 *
 * @param *initSummerTime Estrutura tm com a data de inicio do horario de verão
 * (dia mes e ano apenas).
 * @pram *endSummerTime Estrutura tm com a data de término do horario de verão
 * (dia mes e ano apenas).
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::getSummerTime(tm* initSummerTime, tm* endSummerTime) {
    if (!summerTimeEnable)
        return ERROR;
    initSummerTime->tm_mday = this->initSummerTime.tm_mday;
    initSummerTime->tm_mon = this->initSummerTime.tm_mon;
    initSummerTime->tm_year = this->initSummerTime.tm_year;

    endSummerTime->tm_mday = this->endSummerTime.tm_mday;
    endSummerTime->tm_mon = this->endSummerTime.tm_mon;
    endSummerTime->tm_year = this->endSummerTime.tm_year;
    return OK;
}

/**
 * Configura a data e hora do módulo RTC.
 *
 * @param time Ponteiro para estrutura contendo os dados referentes a data e
 * hora a serem configurados.
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::setDateTime(tm* dateTime) {
    if (errorFlag)
        return ERROR;
    int result = ERROR;
    try {
        this->pause(true);
        usleep(500000);
        mktime(dateTime); //normalizando estrutura
        if (this->sendData(RTC_SEC, BIN_TO_BCD(dateTime->tm_sec)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (this->sendData(RTC_MIN, BIN_TO_BCD(dateTime->tm_min)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (this->sendData(RTC_HOUR, BIN_TO_BCD(dateTime->tm_hour)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (this->sendData(RTC_WDAY, BIN_TO_BCD(dateTime->tm_wday)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (this->sendData(RTC_MDAY, BIN_TO_BCD(dateTime->tm_mday)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (this->sendData(RTC_MON, BIN_TO_BCD(dateTime->tm_mon + 1)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (this->sendData(RTC_YEAR, BIN_TO_BCD(dateTime->tm_year - 100)) != OK) {
            this->pause(false);
            return ERROR;
        }
        if (changedDateTimeEventHandler != NULL) { //lançando evento
            changedDateTimeEventHandler(this->object, &this->rtcTime, dateTime);
        }
        //copiando dados para estrutura interna
        memcpy(&rtcTime, dateTime, sizeof (tm));
        result = OK;
    } catch (...) {

    }
    this->pause(false);
    usleep(5000);
    return result;
}

/**
 * Lê a data e hora da struct da classe RTC.
 *
 * @param time Ponteiro para estrutura contendo os dados referentes a data e
 * hora a serem lidos.
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::getDateTime(tm* dateTime) {
    if (errorFlag)
        return ERROR;
    memcpy(dateTime, &rtcTime, sizeof (tm));
    return OK;
}

/**
 * Ajusta a frequência do módulo.
 *
 * @param freq Valor de ajuste da frequência (-127 ~ 127).
 * @return 0 para OK, -1 para ERRO.
 */
int HRtc::frequencyAdjustment(int frequency) {
    try {
        if (this->debugLevel >= H_DBG_LVL_5) {
            char str[64] = "";
            sprintf(str, "Frequência - %d", frequency);
            debug(str, "");
        }
        frequency = APPLY_LIMIT(frequency, -127, 127);
        if (ds3231_write_byte(&rtc, RTC_AGGING_OFFSET, frequency) >= 0) {
            this->temperatureCompensation();
            return OK;
        }
    } catch (...) {

    }
    return ERROR;
}

/**
 * Finaliza thread em execução.
 *
 * @return 0.
 */
int HRtc::terminate() {
    this->terminated = true;
    return OK;
}

/**
 * Pausa a thread em execução.
 *
 * @param pause true para pausar false para liberar a thread.
 * @return 0.
 */
int HRtc::pause(bool pause) {
    this->paused = pause;
    return OK;
}

/**
 * Habilita a mudança automática de horário de verão.
 *
 * @param active 0 para desativar, 1 para ativar.
 */
void HRtc::autoSummerTime(bool active) {
    summerTimeEnable = active;
}

bool HRtc::isSummerTimeActive() {
    return this->summerTimeEnable;
}

/**
 * Verifica se a thread está rodando.
 *
 * @return 0 para thread parada, 1 para thread ativa.
 */
bool HRtc::isRunning() {
    return this->running;
}

/**
 * Trata os evendos gerados pela thread.
 * @return 0.
 */
int HRtc::setChangedDateTimeEventHandler(void* object,
        void (*function) (void* object, tm* oldtime, tm* newTime)) {
    this->object = object;
    this->changedDateTimeEventHandler = function;
    return 0;
}

/**
 * Obtem instância da classe (singleton).
 *
 * @return instância da classe.
 */
HRtc* HRtc::getInstance() {
    if (hrtc == NULL) {
        hrtc = new HRtc();
    }
    return hrtc;
}

/**
 * Obtem instância da classe (singleton).
 *
 * @return instância da classe.
 */
HRtc* HRtc::getInstance(char* address) {
    if (hrtc == NULL) {
        hrtc = new HRtc(address);
    }
    return hrtc;
}

/**
 * Obtem instância da classe (singleton).
 *
 * @return instância da classe.
 */
HRtc* HRtc::getInstance(tm initSummerTime, tm endSummerTime) {
    if (hrtc == NULL) {
        hrtc = new HRtc(initSummerTime, endSummerTime);
    }
    return hrtc;
}

/**
 * Obtem instância da classe (singleton).
 *
 * @return instância da classe.
 */
HRtc* HRtc::getInstance(char* address, tm initSummerTime, tm endSummerTime) {
    if (hrtc == NULL) {
        hrtc = new HRtc(address, initSummerTime, endSummerTime);
    }
    return hrtc;
}
