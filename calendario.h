/*
 * calendario.h
 *
 *  Created on: 26/07/2013
 *      Author: cuki
 */

#ifndef CALENDARIO_H_
#define CALENDARIO_H_

#define DS1307		0xD0
#define sec_addr	0x00
#define min_addr	0x01
#define hour_addr	0x02
#define day_addr	0x03
#define date_addr	0x04
#define month_addr	0x05
#define year_addr	0x06
#define ctlr_addr	0x07
#define debounce 	100

enum dow {
	dom, seg, ter, qua, qui, sex, sab
};

struct cal {
	short am_pm, _12h;
	int segundos, minutos, horas, dow, dia, mes, ano;
};

int decToBcd(int val) {
	return ((val / 10 * 16) + (val % 10));
}

int bcdToDec(int val) {
	return ((val / 16 * 10) + (val % 16));
}

int getReg(int addr) {
	int leitura;

	i2c_start();
	i2c_write(DS1307);
	i2c_write(addr);
	i2c_start();
	i2c_write(DS1307 + 1);
	leitura = i2c_read(0);
	i2c_stop();

	return leitura;
}

void writeReg(int addr, int dado) {
	i2c_start();
	i2c_write(DS1307);
	i2c_write(addr);
	i2c_write(dado);
	i2c_stop();
}

void pause(void) {
	int aux;

	aux = getReg(sec_addr);
	if (!bit_test(aux, 7))
		aux |= 0x80;
	else
		aux &= 0x7F;
	writeReg(sec_addr, aux);
}

int toSec(int in) {
	int lsb, msb;

	lsb = in & 0x0F;
	msb = (in & 0x70) >> 4;

	return 10 * msb + lsb;
}

int toHour(int in, short *AM_PM, short *formato) {
	int msb, lsb;

	*formato = bit_test(in, 6);
	if (*formato) {
		msb = (in & 0x10) >> 4;
		*AM_PM = bit_test(in, 5);
	} else {
		msb = (in & 0x30) >> 4;
	}
	lsb = in & 0x0F;

	return 10 * msb + lsb;
}

int toDate(int in) {
	int msb, lsb;

	lsb = in & 0x0F;
	msb = (in & 0x30) >> 4;

	return msb * 10 + lsb;
}

int toMonth(int in) {
	int lsb, msb;

	lsb = in & 0x0F;
	msb = (in & 0x10) >> 4;

	return msb * 10 + lsb;
}

int toYear(int in) {
	int lsb, msb;

	lsb = in & 0x0F;
	msb = (in & 0xF0) >> 4;

	return msb * 10 + lsb;
}

int getSec(void) {
	return toSec(getReg(sec_addr));
}

int getMin(void) {
	return toSec(getReg(min_addr));
}

int getHour(short *AM_PM, short *formato) {
	return toHour(getReg(hour_addr), *AM_PM, *formato);
}

int getDayOfWeek(void) {
	return getReg(day_addr) & 0x07;
}

int getDate(void) {
	return getReg(toDate(date_addr));
}

int getMonth(void) {
	return getReg(toMonth(month_addr));
}

int getYear(void) {
	return getReg(toYear(year_addr));
}

short initDS1307(void) {
	int aux;
	short ack = 1;

	i2c_start();
	ack = i2c_write(DS1307);
	i2c_stop();

	if (!ack)
		writeReg(sec_addr, getReg(sec_addr) & 0x7F);

	return ack;
}

void getDS1307(struct cal *calendario) {
	int aux;

	i2c_start();
	i2c_write(DS1307);
	i2c_write(sec_addr);
	i2c_start();
	i2c_write(DS1307 + 1);
	calendario->segundos = toSec(i2c_read());
	calendario->minutos = toSec(i2c_read());
	calendario->horas = toHour(i2c_read(), calendario->am_pm, calendario->_12h);
	calendario->horas = bcdToDec(i2c_read() & 0x63);
	calendario->dow = i2c_read() & 0x07;
	calendario->dia = toDate(i2c_read());
	calendario->mes = toMonth(i2c_read());
	calendario->ano = toYear(i2c_read(0));
	i2c_stop();
}

void setDS1307(struct cal *calen) {
	i2c_start();
	i2c_write(DS1307);
	i2c_write(sec_addr);
	i2c_write(calen->segundos);
	i2c_write(calen->horas);
	i2c_write(calen->dow);
	i2c_write(calen->dia);
	i2c_write(calen->mes);
	i2c_write(calen->ano);
	i2c_write(0);
	i2c_stop();
}

#endif /* CALENDARIO_H_ */
