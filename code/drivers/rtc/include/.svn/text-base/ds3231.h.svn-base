#ifndef _DS3231_H_
#define _DS3231_H_

#include <linux/i2c-dev.h>
#include <linux/i2c.h>

struct ds3231 {
    char *dev; // device file i.e. /dev/i2c/N
    int addr; // i2c address - 0x68
    int fd; // file descriptor
};

int ds3231_open(char *dev_fqn, int addr, struct ds3231 *rtc);
int ds3231_close(struct ds3231 *rtc);
int ds3231_read_byte(struct ds3231 *rtc, __u16 mem_addr);
int ds3231_read_current_byte(struct ds3231 *rtc);
int ds3231_write_byte(struct ds3231 *rtc, __u16 mem_addr, __u8 data);

#endif

