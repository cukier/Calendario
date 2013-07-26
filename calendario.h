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

struct cal {
	short am_pm;
	int segundos, minutos, horas, dow, dia, mes, ano;
};

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

int toHour(int in, short *AM_PM) {
	int msb, lsb;

	if (bit_test(in, 7)) {
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

int getHour(short *AM_PM) {
	return toHour(getReg(hour_addr), *AM_PM);
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

void initDS1307(void) {
	int aux;

	writeReg(sec_addr, getReg(sec_addr) & 0x7F);
}

void getDS1307(struct cal *calendario) {
	i2c_start();
	i2c_write(DS1307);
	i2c_write(sec_addr);
	i2c_start();
	i2c_write(DS1307 + 1);
	calendario->segundos = toSec(i2c_read());
	calendario->minutos = toSec(i2c_read());
	calendario->horas = toHour(i2c_read(), calendario->am_pm);
	calendario->dow = i2c_read() & 0x07;
	calendario->dia = toDate(i2c_read());
	calendario->mes = toMonth(i2c_read());
	calendario->ano = toYear(i2c_read(0));
	i2c_stop();
}

void setDS1307(int seg, int min, int hor, int dow, int dia, int mes, int ano,
		short am_pm) {
	i2c_start();
	i2c_write(DS1307);
	i2c_write(sec_addr);
	i2c_write(seg);
	i2c_write(min);
}

#endif /* CALENDARIO_H_ */
