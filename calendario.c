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

void apagar(int addr) {
	i2c_write(addr);
	i2c_write(sec_addr);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
}

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

void ds1307_init(void) {
	int seconds = 0;

	i2c_start();
	i2c_write(0xD0);      // WR to RTC
	i2c_write(0x00);      // REG 0
	i2c_start();
	i2c_write(0xD1);      // RD from RTC
	seconds = i2c_read(0); // Read current "seconds" in DS1307
	i2c_stop();
	seconds &= 0x7F;

	delay_us(3);

	i2c_start();
	i2c_write(0xD0);      // WR to RTC
	i2c_write(0x00);      // REG 0
	i2c_write(seconds); // Start oscillator with current "seconds value
	i2c_start();
	i2c_write(0xD0);      // WR to RTC
	i2c_write(0x07);      // Control Register
	i2c_write(0x10);     // Disable squarewave output pin
	i2c_stop();

}

int main(void) {

	if (check(disp_addr)) {
		printf("\fErro, nao responde");
		return 1;
	} else {
		printf("\fDisp. OK...");
	}

	ds1307_init();

	while (TRUE) {
		delay_ms(500);
		segundos = ler_dado(sec_addr);
		LSB = (segundos | 0xF0) & 0x0F;
		MSB = ((segundos | 0x0F) & 0x70) >> 4;
		printf("\f%u LSB:%u MSB:%u", segundos, LSB, MSB);
		printf("\n%u%u", MSB, LSB);
	}
	return 0;
}
