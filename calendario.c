///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F252.h>

#fuses HS
#use delay(crystal=15MHz)
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7)
#use i2c(master, sda=PIN_C4, scl=PIN_C3, fast=50000)

#include "calendario.h"

cal_type calendario;

int main(void) {

	int formato[4], AM_PM[3], diaSemana[4], aux;

	delay_ms(100);
	initDS1307();
	delay_ms(1000);
	printf("\fHello");

	while (TRUE) {

		if (!input(PIN_B0)) {
			delay_ms(100);
			if (!input(PIN_B0)) {
				calendario.segundos = 0;
				calendario.minutos = 44;
				calendario.horas = 16;
				calendario.dow = seg;
				calendario.dia = 24;
				calendario.mes = 11;
				calendario.ano = 14;
				calendario._12h = 0;
				calendario.am_pm = 0;
				setDS1307(&calendario);
				delay_ms(500);
			}
		}

		delay_ms(100);
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

		if (aux != calendario.segundos) {
			aux = calendario.segundos;
			printf("\f%s %s  %02d:%02d:%02d", formato, AM_PM, calendario.horas,
					calendario.minutos, calendario.segundos);
			printf("\n%s     %02d/%02d/%02d", diaSemana, calendario.dia,
					calendario.mes, calendario.ano);
		}
//		delay_ms(1000);
	} //infinite loop
	return 0;
} //main
