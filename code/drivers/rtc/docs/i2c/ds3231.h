#ifndef _DS3231_H_
#define _DS3231_H_
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

struct ds3231
{
	char *dev; 	// device file i.e. /dev/i2c/N
	int addr;	// i2c address - 0x68
	int fd;		// file descriptor	
};

/*
 * opens the eeprom device at [dev_fqn] (i.e. /dev/i2c-N) whose address is
 * [addr] and set the eeprom_24c32 [e]
 */
int ds3231_open(char *dev_fqn, int addr, struct ds3231 *rtc);
/*
 * closees the eeprom device [e]
 */
int ds3231_close(struct ds3231 *rtc);
/*
 * read and returns the eeprom byte at memory address [mem_addr]
 * Note: eeprom must have been selected by ioctl(fd,I2C_SLAVE,address)
 */
int ds3231_read_byte(struct ds3231 *rtc, __u16 mem_addr);
/*
 * read the current byte
 * Note: eeprom must have been selected by ioctl(fd,I2C_SLAVE,address)
 */
int ds3231_read_current_byte(struct ds3231 *rtc);
/*
 * writes [data] at memory address [mem_addr]
 * Note: eeprom must have been selected by ioctl(fd,I2C_SLAVE,address)
 */
int ds3231_write_byte(struct ds3231 *rtc, __u16 mem_addr, __u8 data);

#endif

