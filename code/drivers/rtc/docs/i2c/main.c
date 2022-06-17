 #include <unistd.h>
 #include <stdio.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include "ds3231.h"


 

 #define BCD_TO_BIN(val) (((val)&0x0F) + ((val)>>4)*10)
 #define BIN_TO_BCD(val) ((((val)/10)<<4) + (val)%10) 
#define die_if(a, msg) do { do_die_if( a , msg, __LINE__); } while(0);

void do_die_if(int b, char* msg, int line)
{
	if(!b)
		return;
	fprintf(stderr, "Error at line %d: %s\n", line, msg);
	fprintf(stderr, "	sysmsg: %s\n", strerror(errno));
	exit(1);
}

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

static int set_time(struct ds3231 *rtc, int addr, __u8 data)
{
	ds3231_write_byte(rtc, addr, data);
	return 0;
}

static int read_time(struct ds3231 *rtc, int addr)
{
	int ch;	
	die_if((ch = ds3231_read_byte(rtc, addr)) < 0, "read error");		
	fflush(stdout);	
	return ch;
}


/*---- Para converter mês e dia da semana em string----
static char *ds3231_mon2str( unsigned int mon)
 {
     char *mon2str[12] = {
       "Jan", "Feb", "Mar", "Apr", "May", "Jun",
       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
     };
     if( mon > 11) return "error";
     else return mon2str[ mon];
 }
 */

 /*
 static char *ds3231_week2str( unsigned int week)
 {
     char *week2str[7] = {
       "Monday", "Tuesday", "Wednesday", "Tursday", "Friday", "Saturday","Sunday"
     };
     if( week > 6) return "error";
     else return week2str[ week];
 }
  */

struct time_Des
 {
     int second;
     int minute;
     int hour;
     int week;
     int date;
     int month;
     int year;
 };

 static struct time_Des time;
  

 int main(void) {

    char val;
    struct ds3231 rtc;
    int year,month,date,week,hour,minute,second;   
       
    printf("Start test DS1307\n");
    printf("\n\rDo you want to Reset Time(Y/N)");
 
    scanf("%c",&val);
 if(val=='Y'||val=='y')
 {
 
     printf("\n\ryear(00-99):");
     scanf("%2x",&year);
     

     printf("\n\rmonth(1-12):");
     scanf("%2x",&month);
     

     printf("\n\rdate(1-31):");
     scanf("%2x",&date);
     

     printf("\n\rweek(1-7):");
     scanf("%2x",&week);
     

     printf("\n\rhour(0-23):");
     scanf("%2x",&hour);
     

     printf("\n\rminute(0-59):");
     scanf("%2x",&minute);
     

     printf("\n\rsecond(0-59):");
     scanf("%2x",&second);
     

     printf("\nYour input string is =%d %d %d %d %d %d %d\n\r\n\r",year,month,date,week,hour,minute,second);
     
  

     if ((ds3231_open("/dev/i2c/0",0x68,&rtc)) < 0) {
         printf("open /dev/i2c/0 is failed\n");
         exit(1);
    }
     usleep(1000);
     set_time(&rtc, 0, second);
     set_time(&rtc, 1, minute);
     set_time(&rtc, 2, hour);
     set_time(&rtc, 3, date);
     set_time(&rtc, 4, month);
     set_time(&rtc, 5, year);
     set_time(&rtc, 6, week);
     set_time(&rtc, 7, 7);
     

 }
  if(val=='N'||val=='n'){
     if ((ds3231_open("/dev/i2c/0",0x68,&rtc)) < 0) {
         printf("open /dev/i2c/0 is failed\n");
         exit(1);
     }
     usleep(1000);
     set_time(&rtc, 0, 0);
     set_time(&rtc, 1, 1);
     set_time(&rtc, 2, 2);
     set_time(&rtc, 3, 3);
     set_time(&rtc, 4, 4);
     set_time(&rtc, 5, 5);
     set_time(&rtc, 6, 6);
     set_time(&rtc, 7, 7);
     
  }

     while (1)    // Set sequential part here
     {      
     
 //  ds1307Frame.pTime = (char*)&ds1307time;

     time.second = BCD_TO_BIN(read_time(&rtc, 0));
     time.minute = BCD_TO_BIN(read_time(&rtc, 1));
     time.hour = BCD_TO_BIN(read_time(&rtc, 2));
     time.date = BCD_TO_BIN(read_time(&rtc, 3));
     time.month = BCD_TO_BIN(read_time(&rtc, 4));
     time.year = BCD_TO_BIN(read_time(&rtc, 5));
     time.week = BCD_TO_BIN(read_time(&rtc, 6));
     printf("%d:%d:%d\n", time.hour, time.minute, time.second);
     //printf ("Minutos: %d/n",read_time(&rtc, 1));
     //printf ("Hora: %d/n",read_time(&rtc, 2));
     usleep(1000000);
     }


    ds3231_close(&rtc);
    exit(0);
}