///*
// * calendario.c
// *
// *  Created on: 02/02/2012
// *      Author: cuki
// */

#include<18F452.h>                  //Aqui é incluso o header (*.h)  para o microcontrolador utilizado.
//#device ADC = 10                      //Define 10 bits para o resultado da conversão AD
#use delay (clock=8000000)	     //Aqui definimos a frequência do cristal para cálculo dos delays
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP //Configuração dos fusíveis
//#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7)
#include "ds1307.c"
#include "lcd_8b.c"

void main() {
	BYTE sec;
	BYTE min;
	BYTE hrs;
	BYTE day;
	BYTE month;
	BYTE yr;
	BYTE dow;

//	ds1307_init();

	// Set date for -> 04 de abril de 2009 - Sábado
	// Set time for -> 11:07:00
	//ds1307_set_date_time(3, 2, 12, 6, 8, 54, 0);

	lcd_init();

	printf(lcd, "\f");

	while (true) {
		ds1307_get_date(day, month, yr, dow);
		ds1307_get_time(hrs, min, sec);

		lcd_pos_xy(1, 1);
		printf(lcd, "\%02d/\%02d/\%02d", day, month, yr);
		//		lcd_pos_xy(1, 2);
		printf(lcd, "\n");
		printf(lcd, "\%02d:\%02d:\%02d", hrs, min, sec);

		if (!input(PIN_B0)) {
			day++;
			if (day > 31)
				day = 1;
			ds1307_set_date_time(day, month, yr, dow, hrs, min, sec);
			delay_ms(300);
		}
		if (!input(PIN_C0)) {
			month++;
			if (month > 12)
				month = 1;
			ds1307_set_date_time(day, month, yr, dow, hrs, min, sec);
			delay_ms(300);
		}
		if (!input(PIN_B1)) {
			hrs++;
			if (hrs > 23)
				hrs = 0;
			ds1307_set_date_time(day, month, yr, dow, hrs, min, sec);
			delay_ms(300);
		}
		if (!input(PIN_B2)) {
			min++;
			if (min > 59)
				min = 0;
			ds1307_set_date_time(day, month, yr, dow, hrs, min, sec);
			delay_ms(300);
		}
		delay_ms(500);
	}
}

//#include <18F452.h>
//
//#fuses hs
//#use delay (clock = 8MHz, RESTART_WDT)
//#use i2c (master,scl=pin_c3,sda=pin_c4,force_hw,fast = 100000)
//
//#include "lcd_8b.c"
////#include "i2c.h"
//#include <stdlib.h>
//
//#define _SECONDS    0x00
//#define _MINUTES    0x01
//#define _HOURS    0x02
//#define _DAY      0x03
//#define _DATE    0x04
//#define _MONTH    0x05
//#define _YEAR    0x06
//#define _CONTROL    0x07
//#define _MODO12    0x00
//#define _MODO24    0x40
//
//void setMode(int modo);
//int make4(int var, int nibble);
//BYTE bin2bcd(BYTE binary_value);
//BYTE bcd2bin(BYTE bcd_value);
//void ds1307_set_date_time(BYTE day, BYTE mth, BYTE year, BYTE dow, BYTE hr,
//		BYTE min, BYTE sec);
//
//struct {
//	int seconds;
//	int minutes;
//	int hours;
//	int PM;
//	int modo24;
//	int day;
//	int date;
//	int month;
//	int year;
//	int sqwe_rs;
//} tempo;
//
//char semana[];
//char modo[];
//
//#INT_TIMER2
//void isr_timer2() {
//	register int cont;
//	register int aux;
//
//	i2c_start();
//	i2c_write(0xD1);
//	i2c_write(0x00);
//	for (cont = 0x00; cont <= 0x07; ++cont) {
//		if (cont == 0x07) {
//			aux = i2c_read(0);
//			i2c_stop();
//		} else
//			aux = i2c_read();
//		if (aux != 0xFF) {
//			switch (aux) {
//			case _SECONDS:
//				tempo.seconds = make4(aux, 1) * 10 + make4(aux, 0);
//				break;
//			case _MINUTES:
//				tempo.minutes = 10 * make4(aux, 1) + make4(aux, 0);
//				break;
//			case _HOURS:
//				tempo.hours = make4(aux, 0);
//				if (bit_test(aux, 6)) {
//					tempo.hours += bit_test(aux, 4) * 10;
//					tempo.PM = bit_test(aux, 5);
//					tempo.modo24 = 0;
//				} else {
//					tempo.hours += (0x30 && make4(aux, 1)) * 10;
//					tempo.modo24 = 1;
//				}
//				break;
//			case _DAY:
//				tempo.day = make4(aux, 0);
//				break;
//			case _DATE:
//				tempo.date = make4(aux, 1) * 10 + make4(aux, 0);
//				break;
//			case _MONTH:
//				tempo.month = make4(aux, 1) * 10 + make4(aux, 0);
//				break;
//			case _YEAR:
//				tempo.year = make4(aux, 1) * 10 + make4(aux, 0);
//				break;
//			default:
//			}
//		}
//	}
//}
//
//void main(void) {
//
//	delay_ms(15);
//	lcd_init();
//
//	setMode(_MODO24);
//	ds1307_set_date_time(04, 4, 9, 6, 11, 07, 00);
//
//	setup_timer_2(T2_DIV_BY_16, 255, 16);
//	enable_interrupts(INT_TIMER2);
////	enable_interrupts(GLOBAL);
//
//	while (true) {
//
//		switch (tempo.day) {
//		case 0:
//			strcpy(semana, "DOM");
//			break;
//		case 1:
//			strcpy(semana, "SEG");
//			break;
//		case 2:
//			strcpy(semana, "TER");
//			break;
//		case 3:
//			strcpy(semana, "QUA");
//			break;
//		case 4:
//			strcpy(semana, "QUI");
//			break;
//		case 5:
//			strcpy(semana, "SEX");
//			break;
//		case 6:
//			strcpy(semana, "SAB");
//			break;
//		default:
//			strcpy(semana, "NON");
//		}
//
//		if (!tempo.modo24) {
//			if (tempo.PM)
//				strcpy(modo, "PM");
//			else
//				strcpy(modo, "AM");
//		} else {
//			strcpy(modo, "  ");
//		}
//
//		printf(lcd, "\f%3s %02d/%02d/%04d", semana, tempo.date, tempo.month,
//				tempo.year);
//		printf(lcd, "\n%2s %02d:%02d:%02d", modo, tempo.hours, tempo.minutes,
//				tempo.seconds);
//		delay_ms(500);
//	}//infinit loop
//}//this is the end, beautyfull friend, the end!
//
//void setMode(int mode) {
//	BYTE seconds = 0;
//
//	i2c_start();
//	i2c_write(0xD0); // WR to RTC
//	i2c_write(0x00); // REG 0
//	i2c_start();
//	i2c_write(0xD1); // RD from RTC
//	seconds = bcd2bin(i2c_read(0)); // Read current "seconds" in DS1307
//	i2c_stop();
//	seconds &= 0x7F;
//
//	delay_us(3);
//
//	i2c_start();
//	i2c_write(0xD0); // WR to RTC
//	i2c_write(0x00); // REG 0
//	i2c_write(bin2bcd(seconds)); // Start oscillator with current "seconds value
//	i2c_start();
//	i2c_write(0xD0); // WR to RTC
//	i2c_write(0x07); // Control Register
//	i2c_write(0x10); // Disable squarewave output pin
//	i2c_stop();
//
//}
//
//int make4(int var, int nibble) {
//	register int retorno;
//
//	if (nibble)
//		retorno = var >> 4;
//	else
//		retorno = var;
//
//	retorno &= 0x0F;
//
//	return retorno;
//
//}
//
//BYTE bin2bcd(BYTE binary_value) {
//	BYTE temp;
//	BYTE retval;
//
//	temp = binary_value;
//	retval = 0;
//
//	while (true) {
//		// Get the tens digit by doing multiple subtraction
//		// of 10 from the binary value.
//		if (temp >= 10) {
//			temp -= 10;
//			retval += 0x10;
//		} else // Get the ones digit by adding the remainder.
//		{
//			retval += temp;
//			break;
//		}
//	}
//
//	return (retval);
//}
//
//BYTE bcd2bin(BYTE bcd_value) {
//	BYTE temp;
//
//	temp = bcd_value;
//	// Shifting upper digit right by 1 is same as multiplying by 8.
//	temp >>= 1;
//	// Isolate the bits for the upper digit.
//	temp &= 0x78;
//
//	// Now return: (Tens * 8) + (Tens * 2) + Ones
//
//	return (temp + (temp >> 2) + (bcd_value & 0x0f));
//}
//
//void ds1307_set_date_time(BYTE day, BYTE mth, BYTE year, BYTE dow, BYTE hr,
//		BYTE min, BYTE sec) {
//	sec &= 0x7F;
//	hr &= 0x3F;
//
//	i2c_start();
//	i2c_write(0xD0); // I2C write address
//	delay_us(1);
//	i2c_write(0x00); // Start at REG 0 - Seconds
//	i2c_write(bin2bcd(sec)); // REG 0
//	i2c_write(bin2bcd(min)); // REG 1
//	i2c_write(bin2bcd(hr)); // REG 2
//	i2c_write(bin2bcd(dow)); // REG 3
//	i2c_write(bin2bcd(day)); // REG 4
//	i2c_write(bin2bcd(mth)); // REG 5
//	i2c_write(bin2bcd(year)); // REG 6
//	//i2c_write(0x80);            // REG 7 - Disable squarewave output pin
//	i2c_write(0x10);
//	i2c_stop();
//}
//
////int addr[7];
////int cont = 0;
////
////i2c_start();
////i2c_write(0xD1);
////i2c_write(0x00);
////addr[0] = i2c_read();
////addr[1] = i2c_read();
////addr[2] = i2c_read();
////addr[3] = i2c_read();
////addr[4] = i2c_read();
////addr[5] = i2c_read();
////addr[6] = i2c_read();
////addr[7] = i2c_read(0);
////i2c_stop();
//
////printf(lcd, "\f");
////for (cont = 0; cont < 7; ++cont) {
////	if (cont == 5)
////		printf(lcd, "\n");
////	printf(lcd, "%x ", addr[cont]);
////
////}
