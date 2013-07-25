///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F452.h>

#fuses H4
#use delay (crystal=8MHz, clock=32MHz)
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7)
#use i2c(master, sda=PIN_C4, scl=PIN_C3, fast=50000)

#define disp_addr	0xD0
#define sec_addr	0x00

int segundos, LSB, MSB;

short check(int addr) {
	short ack = 1;
	i2c_start();
	ack = i2c_write(addr);
	i2c_stop();
	return ack;
}

int ler_dado(int addr) {
	int leitura = 0;

	i2c_start();
	i2c_write(disp_addr);
	i2c_write(addr);
	i2c_start();
	i2c_write(disp_addr + 1);
	leitura = i2c_read(0);
	i2c_stop();
	return leitura;
}

int main(void) {

	if (check(disp_addr)) {
		printf("\fErro, nao responde");
		return 1;
	} else {
		printf("\fDisp. OK...");
	}

	while (TRUE) {
		delay_ms(500);
		segundos = ler_dado(sec_addr);
		LSB = segundos & 0x0F;
		MSB = (segundos & 0x70) >> 4;
		printf("\f%u LSB:%u MSB:%u", segundos, LSB, MSB);
		printf("\n%u%u", MSB, LSB);
	}
	return 0;
}
