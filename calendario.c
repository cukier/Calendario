///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F452.h>

#fuses H4
#use delay (crystal=10MHz, clock=40MHz)
//#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7)
#use i2c(master, sda=PIN_C4, scl=PIN_C3, fast=50000)

#include"calendario.h"
#include"lcd_4b.c"

struct cal calendario;

int main(void) {

	int formato[4], AM_PM[3], diaSemana[4];
	short ack = 0;

	lcd_init();
	delay_ms(100);
	initDS1307();

	while (TRUE) {

		if (!input(PIN_B0)) {
			delay_ms(100);
			if (!input(PIN_B0)) {
				calendario.segundos = 0;
				calendario.minutos = 47;
				calendario.horas = 14;
				calendario.dow = seg;
				calendario.dia = 29;
				calendario.mes = 7;
				calendario.ano = 13;
				calendario._12h = 0;
				calendario.am_pm = 0;
				setDS1307(&calendario);
				delay_ms(500);
			}
		}

		delay_ms(500);
		getDS1307(&calendario);

		if (calendario._12h) {
			strcpy(formato, "12h");
			if (calendario.am_pm)
				strcpy(AM_PM, "AM");
			else
				strcpy(AM_PM, "PM");
		} else {
			strcpy(formato, "24h");
			strcpy(AM_PM, "  ");
		}
		AM_PM[2] = '\0';
		formato[3] = '\0';

		switch (calendario.dow) {
		case dom:
			strcpy(diaSemana, "DOM");
			break;
		case seg:
			strcpy(diaSemana, "SEG");
			break;
		case ter:
			strcpy(diaSemana, "TER");
			break;
		case qua:
			strcpy(diaSemana, "QUA");
			break;
		case qui:
			strcpy(diaSemana, "QUI");
			break;
		case sex:
			strcpy(diaSemana, "SEX");
			break;
		case sab:
			strcpy(diaSemana, "SAB");
			break;
		default:
			strcpy(diaSemana, "ERR");
			break;
		}
		diaSemana[3] = '\0';

		printf(lcd, "\f%s %s  %02d:%02d:%02d", formato, AM_PM, calendario.horas,
				calendario.minutos, calendario.segundos);
		printf(lcd, "\n%s     %02d/%02d/%02d", diaSemana, calendario.dia,
				calendario.mes, calendario.ano);
	} //infinite loop
} //main
