///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<16F628A.h>

#fuses INTRC_IO
#use delay(clock=4MHz)
#use rs232(baud=9600,xmit=PIN_B2)
#use i2c(master, sda=PIN_B4, scl=PIN_B5, fast=50000, force_sw)

#include "calendario.h"

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
			printf("\f%s %s  %02d:%02d:%02d", formato, AM_PM, calendario.horas,
					calendario.minutos, calendario.segundos);
			printf("\n%s     %02d/%02d/%02d", diaSemana, calendario.dia,
					calendario.mes, calendario.ano);
		}
//		delay_ms(1000);
	} //infinite loop
	return 0;
} //main
