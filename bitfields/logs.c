#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#define TS_BUF_LENGHT 50

#define LOG_TIME  	 0x01	/* 0b00000001 */
#define LOG_DATE  	 0x02	/* 0b00000010 */
#define LOG_USER  	 0x04	/* 0b00000100 */
#define LOG_COUNT 	 0x08  	/* 0b00001000 */
#define LOG_ALL   	 0xFF  	/* 0xFFFFFFFF */
#define LOG_FULLTIME 0x03   /* 0x00000011 */

void log_msg(FILE *fp, char *msg, uint8_t options) 
{
	time_t time_val;
	static uint64_t log_count = 0;
	char time_stamp[TS_BUF_LENGHT];
	char date_stamp[TS_BUF_LENGHT];
	struct tm *tm_info;

	time_val = time(0);
	tm_info = localtime(&time_val);

	strftime(date_stamp, TS_BUF_LENGHT, "%F (%a)", tm_info);
	strftime(time_stamp, TS_BUF_LENGHT, "%H:%M:%S", tm_info);
	
	if (options & LOG_COUNT) 
		fprintf(fp, "%li, ", ++log_count);
	if (options & LOG_DATE)
		fprintf(fp, "%s, ", date_stamp);
	if (options & LOG_TIME) 
		fprintf(fp, "%s, ", time_stamp);
	if (options & LOG_USER) 
		fprintf(fp, "%s, ", getlogin());
	
	fprintf(fp, "%s\n", msg);
}

