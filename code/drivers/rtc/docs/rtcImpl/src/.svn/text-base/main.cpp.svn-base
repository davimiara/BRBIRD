/*
 *		HENRY EQUIPAMENTOS ELETRÔNICOS E SISTEMAS LTDA
 *
 *   Arquivo de teste da libhrtc.so
 *
 * Autor: Felipe dos Santos
 * Data: 06/2010
 *
 * Modificações:
 *
 * ------------------------------------===--------------------------------------
 * Autor:
 * Data :
 * Alteração :
 * ------------------------------------===--------------------------------------
 * Autor:
 * Data :
 * Alteração :
 * ------------------------------------===--------------------------------------
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../../../include/rtc.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define BUILD_VERSION 1

#define OK 0
#define ERROR -1

void eventoRtc(void* object, tm* oldtime, tm* newTime) {
    printf("\n====Data alterada====\n");

    printf("Data antiga:\n");
    printf("sec = %02d\n", oldtime->tm_sec);
    printf("min = %02d\n", oldtime->tm_min);
    printf("hour = %02d\n", oldtime->tm_hour);
    printf("weekday = %d\n", oldtime->tm_wday + 1);
    printf("day = %d\n", oldtime->tm_mday);
    printf("month = %02d\n", oldtime->tm_mon + 1);
    printf("year = %04d\n", oldtime->tm_year + 1900);

    printf("\nData nova:\n");
    printf("sec = %02d\n", newTime->tm_sec);
    printf("min = %02d\n", newTime->tm_min);
    printf("hour = %02d\n", newTime->tm_hour);
    printf("weekday = %d\n", newTime->tm_wday + 1);
    printf("day = %d\n", newTime->tm_mday);
    printf("month = %02d\n", newTime->tm_mon + 1);
    printf("year = %04d\n", newTime->tm_year + 1900);
}

int main(int argc, char **argv) {
    HRtc* rtc = HRtc::getInstance((char*) "/dev/i2c/0");
    tm time1;
    tm time2;
    bool sair = false;
    if (rtc->connect() == OK) {
        printf("Conectado ao rtc\n");
        rtc->setChangedDateTimeEventHandler(rtc, &eventoRtc);
    } else {
        sair = true;
        printf("Falha ao conectar ao rtc\n");
    }


    while (!sair) {
        printf("\n");
        printf("\n");
        printf("1: freqAdjust\n");
        printf("2: setDateTime\n");
        printf("3: getDateTime\n");
        printf("4: Ajustar horario de verão\n");
        printf("5: Verificar horario de verão configurado\n");
        printf("6: Horario de verao automatico\n");
        printf("0: Sair\n");
        printf("Selecione um comando: ");

        char opt = 0;
        while (opt < '0' || opt > '6')
            opt = getchar();
        printf("\n");
        switch (opt) {
            case '0':
                sair = true;
                break;
            case '1':
                int freq;
                printf("\n\rfreq(-127 ~ 127):\n");
                scanf("%d", &freq);
                if (rtc->frequencyAdjustment(freq) == OK)
                    printf("--------------freqAdjust OK\n");
                break;
            case '2':
                int aux;
                printf("\n\ryear(2000-2999):\n");
                scanf("%d", &aux);
                time1.tm_year = aux - 1900;

                printf("\n\rmonth(1-12):");
                scanf("%d", &aux);
                time1.tm_mon = aux - 1;

                printf("\n\rdate(1-31):");
                scanf("%d", &time1.tm_mday);

                time1.tm_wday = 1;

                printf("\n\rhour(0-23):");
                scanf("%d", &time1.tm_hour);

                printf("\n\rminute(0-59):");
                scanf("%d", &time1.tm_min);

                printf("\n\rsecond(0-59):");
                scanf("%d", &time1.tm_sec);
                printf("\n\n");

                if (rtc->setDateTime(&time1) == OK)
                    printf("-------------setDateTime OK\n");
                else
                    printf("-------------setDateTime ERROR*************\n");
                break;
            case '3':
                if (rtc->getDateTime(&time2) == OK) {
                    printf("-------------getDateTime OK\n");
                    printf("sec = %02d\n", time2.tm_sec);
                    printf("min = %02d\n", time2.tm_min);
                    printf("hour = %02d\n", time2.tm_hour);
                    printf("weekday = %d\n", time2.tm_wday + 1);
                    printf("day = %d\n", time2.tm_mday);
                    printf("month = %02d\n", time2.tm_mon + 1);
                    printf("year = %04d\n", time2.tm_year + 1900);
                } else
                    printf("-------------getDateTime ERROR*************\n");
                break;
            case '4':
                aux = 0;
                printf("====================");
                printf("Dia de Inicio: ================\n");

                printf("\n\ryear(2000-2999):\n");
                scanf("%d", &aux);
                time1.tm_year = aux - 1900;

                printf("\n\rmonth(1-12):");
                scanf("%d", &aux);
                time1.tm_mon = aux - 1;

                printf("\n\rdate(1-31):");
                scanf("%d", &time1.tm_mday);

                printf("\n\n");

                printf("====================");
                printf("Dia de Termino: ================\n");

                printf("\n\ryear(2000-2999):\n");
                scanf("%d", &aux);
                time2.tm_year = aux - 1900;

                printf("\n\rmonth(1-12):");
                scanf("%d", &aux);
                time2.tm_mon = aux - 1;

                printf("\n\rdate(1-31):");
                scanf("%d", &time2.tm_mday);
                printf("\n\n");

                if (rtc->adjustSummerTime(time1, time2) == OK)
                    printf("Horario de verao configurado com sucesso\n");
                else
                    printf("Erro ao configurar Horario de verao\n");

                break;
            case '5':
                if (rtc->getSummerTime(&time1, &time2) == OK) {
                    printf("Inicio: %2d/%2d/%4d\n", time1.tm_mday,
                            time1.tm_mon + 1, time1.tm_year + 1900);
                    printf("Termino: %2d/%2d/%4d\n", time2.tm_mday,
                            time2.tm_mon + 1, time2.tm_year + 1900);
                } else
                    printf("Horario de verao nao configurado\n");
                break;
            case '6':
                printf("\n\r 1 para ativar, 0 para desativar:");
                scanf("%d", &aux);
                rtc->autoSummerTime((bool) aux);
            default: printf("Opção inválida.\n");
                break;
        }
    }
    exit(OK);
} // end main
;