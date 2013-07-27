///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F452.h>

#fuses H4
#use delay (crystal=10MHz, clock=40MHz)fdasfdfd
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7)
#use i2c(master, sda=PIN_C4, scl=PIN_C3, fast=50000)

#include"calendario.h"
#include"lcd_4b.c"

struct cal calendario;

int main(void) {

	int aux;

	initDS1307();
	lcd_init();
	delay_ms(100);fjopedwafjdas~fjkd
//	getDS1307(&calendario);

	while (TRUE) {
		delay_ms(500);
		getDS1307(&calendario);
		printf(lcd, "\f        %02d:%02d:%02d\n        %02d/%02d/%02d",
				calendario.horas, calendario.minutos, calendario.segundos,
				calendario.dia, calendario.mes, calendario.ano);
	}
}
