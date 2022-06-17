#ifndef _RTC_H
#define	_RTC_H

#include <henry/general/hthread.h>
#include "ds3231.h"


#define DEF_I2CBUS "/dev/i2c/0"

class HRtc : private Hthread {
private:
    HRtc();
    HRtc(char* address);
    HRtc(tm initSummerTime, tm endSummerTime);
    HRtc(char* address, tm initSummerTime, tm endSummerTime);

public:
    virtual ~HRtc();

private:
    struct ds3231 rtc;
    tm rtcTime, initSummerTime, endSummerTime;
    bool terminated, paused, errorFlag, summer, unSummer, running;
    bool summerTimeEnable;
    char device[128];
    int debugLevel;

    int initialize();
    int sendData(int adress, unsigned int value);
    int receiveData(int adress);
    int temperatureCompensation();
    bool busy();
    int exec();
    int getDateTime();
    int setSummerTime();
    int unSetSummerTime();
    void* object; //objeto do evento
    void (*changedDateTimeEventHandler)(void* object, tm* oldtime, tm* newTime);

public:
    int connect();
    int connect(char* address);
    int disconnect();
    int adjustSummerTime(tm initSummerTime, tm endSummerTime);
    int getSummerTime(tm* initSummerTime, tm* endSummerTime);
    int setDateTime(tm* time);
    int getDateTime(tm* time);
    int frequencyAdjustment(int freq);
    int terminate();
    int pause(bool pause);
    void autoSummerTime(bool active);
    bool isSummerTimeActive();
    bool isRunning();
     int setChangedDateTimeEventHandler(void* object,
        void (*function) (void* object, tm* oldtime, tm* newTime));
    
    static HRtc* getInstance();
    static HRtc* getInstance(char* address);
    static HRtc* getInstance(tm initSummerTime, tm endSummerTime);
    static HRtc* getInstance(char* address, tm initSummerTime, tm endSummerTime);
};

#endif	/* _RTC_H */
