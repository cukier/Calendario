///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F252.h>

#fuses HS
#use delay(clock=8MHz)
#use rs232(uart1, baud=19200)
#use i2c(master, i2c1, fast=50000, force_hw)

#include "calendario.h"

#define texto1 "\f%s %s  %02d:%02d:%02d"
#define texto2 "\n%s     %02d/%02d/%02d"

cal_type calendario;
int formato[4], AM_PM[3], diaSemana[4], aux;

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

		getDayofWeekExt(diaSemana, &calendario);
		diaSemana[3] = '\0';

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
