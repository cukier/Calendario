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
#define dow_addr	0x03
#define date_addr	0x04
#define month_addr	0x05
#define year_addr	0x06
#define ctlr_addr	0x07
#define debounce 	100

typedef enum dias_da_semana {
	dom = 1, seg, ter, qua, qui, sex, sab
} dia_semana;

typedef struct str_cal {
	short am_pm, _12h;
	int segundos, minutos, horas, dow, dia, mes, ano;
} cal_type;

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

int toHour(int in, short **AM_PM, short **formato) {
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
	return toHour(getReg(hour_addr), &AM_PM, &formato);
}

int getDayOfWeek(void) {
	return getReg(dow_addr) & 0x07;
}

int getDayofWeekExt(int *str, cal_type *calendario) {

	switch (calendario->dow) {
	case dom:
		strcpy(str, "DOM");
		break;
	case seg:
		strcpy(str, "SEG");
		break;
	case ter:
		strcpy(str, "TER");
		break;
	case qua:
		strcpy(str, "QUA");
		break;
	case qui:
		strcpy(str, "QUI");
		break;
	case sex:
		strcpy(str, "SEX");
		break;
	case sab:
		strcpy(str, "SAB");
		break;
	default:
		strcpy(str, "ERR");
		break;
	}

	return getDayOfWeek();
}

void getFormato(int *formato, int *AM_PM, cal_type *calendario) {
	if (calendario->_12h) {
		strcpy(formato, "12h");
		if (calendario->am_pm)
			strcpy(AM_PM, "AM");
		else
			strcpy(AM_PM, "PM");
	} else {
		strcpy(formato, "24h");
		strcpy(AM_PM, "  ");
	}
}

int getDate(void) {
	return toDate(getReg(date_addr));
}

int getMonth(void) {
	return toMonth(getReg(month_addr));
}

int getYear(void) {
	return toYear(getReg(year_addr));
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

void getDS1307(cal_type *calendario) {
	calendario->segundos = getSec();
	calendario->minutos = getMin();
	calendario->horas = getHour(calendario->am_pm, calendario->_12h);
	calendario->dow = getDayOfWeek();
	calendario->dia = getDate();
	calendario->mes = getMonth();
	calendario->ano = getYear();
}

int toBcd(int in) {
	int unidade, dezena;

	dezena = in / 10;
	unidade = in - dezena * 10;

	return (dezena << 4) | unidade;
}

void setDS1307(cal_type *calen) {

	writeReg(sec_addr, toBcd(calen->segundos & 0x7F));
	delay_ms(1);

	writeReg(min_addr, toBcd(calen->minutos & 0x7F));
	delay_ms(1);

	writeReg(hour_addr,
			((int) calen->_12h << 6) | ((int) calen->am_pm << 5)
					| toBcd(calen->horas & 0x3F));
	delay_ms(1);

	writeReg(dow_addr, calen->dow & 0x07);
	delay_ms(1);

	writeReg(date_addr, toBcd(calen->dia & 0x3F));
	delay_ms(1);

	writeReg(month_addr, toBcd(calen->mes & 0x1F));
	delay_ms(1);

	writeReg(year_addr, toBcd(calen->ano));
	delay_ms(1);

	writeReg(0x07, 0x00);
}

#endif /* CALENDARIO_H_ */
