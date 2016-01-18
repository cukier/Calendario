///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F252.h>

#fuses HS
#use delay(clock=8MHz)
#use rs232(uart1, baud=9600)
#use i2c(master, i2c1, fast=1000000, force_hw)

#include "calendario.h"

cal_t calendario;
//int formato[4], AM_PM[3], diaSemana[4], aux;
int formato[4], AM_PM[3], aux;
char *diaSemana;

int main(void) {

	delay_ms(100);
	initDS1307();

	while (TRUE) {

		if (!input(PIN_B0)) {
			delay_ms(100);
			if (!input(PIN_B0)) {
				calendario.segundos = 0;
				calendario.minutos = 26;
				calendario.horas = 13;
				calendario.dow = seg;
				calendario.dia = 2;
				calendario.mes = 3;
				calendario.ano = 15;
				calendario._12h = 0;
				calendario.am_pm = 0;
				setDS1307(&calendario);
				delay_ms(500);
			}
		}

		delay_ms(100);
		getDS1307(&calendario);

		getFormato(formato, AM_PM, &calendario);

		AM_PM[2] = '\0';
		formato[3] = '\0';

		diaSemana = getDayofWeekExt(calendario);
//		strcpy(diaSemana, getDayofWeekExt(calendario));

		if (aux != calendario.segundos) {
			aux = calendario.segundos;
			printf(texto1, formato, AM_PM, calendario.horas, calendario.minutos,
					calendario.segundos);
			printf(texto2, diaSemana, calendario.dia, calendario.mes,
					calendario.ano);
		}
		//		delay_ms(1000);
	} //infinite loop
	return 0;
} //main

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

	aux = getReg(SEC_ADDR);
	if (!bit_test(aux, 7))
		aux |= 0x80;
	else
		aux &= 0x7F;
	writeReg(SEC_ADDR, aux);
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
	return toSec(getReg(SEC_ADDR));
}

int getMin(void) {
	return toSec(getReg(MIN_ADDR));
}

int getHour(short *AM_PM, short *formato) {
	return toHour(getReg(HOUR_ADDR), &AM_PM, &formato);
}

int getDayOfWeek(void) {
	return getReg(DOW_ADDR) & 0x07;
}

char *getDayofWeekExt(cal_t calendario) {
	char *str;

	strcpy(str, "NON");
	switch (calendario.dow) {
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
	str[3] = '\0';

	return str;
}

void getFormato(int *formato, int *AM_PM, cal_t *calendario) {
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
	return toDate(getReg(DATE_ADDR));
}

int getMonth(void) {
	return toMonth(getReg(MONTH_ADDR));
}

int getYear(void) {
	return toYear(getReg(YEAR_ADDR));
}

short initDS1307(void) {
	int aux;
	short ack = 1;

	i2c_start();
	ack = i2c_write(DS1307);
	i2c_stop();

	if (!ack)
		writeReg(SEC_ADDR, getReg(SEC_ADDR) & 0x7F);

	return ack;
}

void getDS1307(cal_t *calendario) {
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

void setDS1307(cal_t *calen) {

	writeReg(SEC_ADDR, toBcd(calen->segundos & 0x7F));
	delay_ms(1);

	writeReg(MIN_ADDR, toBcd(calen->minutos & 0x7F));
	delay_ms(1);

	writeReg(HOUR_ADDR,
			((int) calen->_12h << 6) | ((int) calen->am_pm << 5)
					| toBcd(calen->horas & 0x3F));
	delay_ms(1);

	writeReg(DOW_ADDR, calen->dow & 0x07);
	delay_ms(1);

	writeReg(DATE_ADDR, toBcd(calen->dia & 0x3F));
	delay_ms(1);

	writeReg(MONTH_ADDR, toBcd(calen->mes & 0x1F));
	delay_ms(1);

	writeReg(YEAR_ADDR, toBcd(calen->ano));
	delay_ms(1);

	writeReg(0x07, 0x00);
}
