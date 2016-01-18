/*
 * calendario.h
 *
 *  Created on: 26/07/2013
 *      Author: cuki
 */

#ifndef CALENDARIO_H_
#define CALENDARIO_H_

#define DS1307		0xD0
#define SEC_ADDR	0x00
#define MIN_ADDR	0x01
#define HOUR_ADDR	0x02
#define DOW_ADDR	0x03
#define DATE_ADDR	0x04
#define MONTH_ADDR	0x05
#define YEAR_ADDR	0x06
#define CTLR_ADDR	0x07
#define DEBOUNCE 	100

#ifndef PRINT_SERIAL
#define texto1 "\f%s %s  %02d:%02d:%02d"
#define texto2 "\n%s     %02d/%02d/%02d"
#else
#define texto1 "%s %s %02d:%02d:%02d\n\r"
#define texto2 "%s      %02d/%02d/%02d\n\r"
#endif

typedef enum dow_e {
	dom = 1, seg, ter, qua, qui, sex, sab
} dow_t;

typedef struct cal_s {
	short am_pm, _12h;
	int segundos, minutos, horas, dow, dia, mes, ano;
} cal_t;

int decToBcd(int val);
int bcdToDec(int val);
int getReg(int addr);
void writeReg(int addr, int dado);
void pause(void);
int toSec(int in);
int toHour(int in, short **AM_PM, short **formato);
int toDate(int in);
int toMonth(int in);
int toYear(int in);
int getSec(void);
int getMin(void);
int getHour(short *AM_PM, short *formato);
int getDayOfWeek(void);
char *getDayofWeekExt(cal_t calendario);
void getFormato(int *formato, int *AM_PM, cal_t *calendario);
int getDate(void);
int getMonth(void);
int getYear(void);
short initDS1307(void);
void getDS1307(cal_t *calendario);
int toBcd(int in);
void setDS1307(cal_t *calen);

#endif /* CALENDARIO_H_ */
