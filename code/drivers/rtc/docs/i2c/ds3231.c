
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include "ds3231.h"


static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}

static inline __s32 i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

static inline __s32 i2c_smbus_write_byte(int file, __u8 value)
{
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,value,
	                        I2C_SMBUS_BYTE,NULL);
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command,
                                              __u8 value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BYTE_DATA, &data);
}

static int i2c_write_1b(struct ds3231 *rtc, __u8 buf)
{
	int r;
	// we must simulate a plain I2C byte write with SMBus functions
	r = i2c_smbus_write_byte(rtc->fd, buf);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_1b: %s\n", strerror(errno));
	usleep(10);
	return r;
}

static int i2c_write_2b(struct ds3231 *rtc, __u8 buf[2])
{
	int r;
	// we must simulate a plain I2C byte write with SMBus functions
	r = i2c_smbus_write_byte_data(rtc->fd, buf[0], buf[1]);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_2b: %s\n", strerror(errno));
	usleep(10);
	return r;
}


#define CHECK_I2C_FUNC( var, label ) \
	do { 	if(0 == (var & label)) { \
		fprintf(stderr, "\nError: " \
			#label " function is required. Program halted.\n\n"); \
		exit(1); } \
	} while(0);

int ds3231_open(char *dev_fqn, int addr, struct ds3231 *rtc)
{
	int funcs, fd, r;
	rtc->fd = rtc->addr = 0;
	rtc->dev = 0;

	fd = open(dev_fqn, O_RDWR);
	if(fd <= 0)
	{
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	// get funcs list
	if((r = ioctl(fd, I2C_FUNCS, &funcs) < 0))
	{
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	// check for req funcs
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_WORD_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA );

	// set working device
	if( ( r = ioctl(fd, I2C_SLAVE, addr)) < 0)
	{
		fprintf(stderr, "Error ds3231_open: %s\n", strerror(errno));
		return -1;
	}
	rtc->fd = fd;
	rtc->addr = addr;
	rtc->dev = dev_fqn;
	return 0;
}

int ds3231_close(struct ds3231 *rtc)
{
	close(rtc->fd);
	rtc->fd = -1;
	rtc->dev = 0;
	return 0;
}


int ds3231_read_current_byte(struct ds3231 *rtc)
{
	ioctl(rtc->fd, BLKFLSBUF); // clear kernel read buffer
	return i2c_smbus_read_byte(rtc->fd);
}

int ds3231_read_byte(struct ds3231 *rtc, __u16 mem_addr)
{
	int r;
	ioctl(rtc->fd, BLKFLSBUF); // clear kernel read buffer
	__u8 buf =  mem_addr & 0x0ff;
	r = i2c_write_1b(rtc, buf);
	if (r < 0)
		return r;
	r = i2c_smbus_read_byte(rtc->fd);
	return r;
}

int ds3231_write_byte(struct ds3231 *rtc, __u16 mem_addr, __u8 data){
	
		__u8 buf[2] = { mem_addr & 0x00ff, data };
		return i2c_write_2b(rtc, buf);	
}