#include <18F252.h>

#fuses H4
#use delay(crystal=8MHz, clock=32MHz)
#use rs232(baud=9660, xmit=PIN_C6)
#use i2c(master, sda=PIN_C4, scl=PIN_C3)

#define DS1307_ADD 0xD0

int decToBcd(int val) {
	return ((val / 10 * 16) + (val % 10));
}

int bcdToDec(int val) {
	return ((val / 16 * 10) + (val % 16));
}

void setDateDs1307(int second, int minute, int hour, int dayOfWeek,
		int dayOfMonth, int month, int year) {
	i2c_start();
	i2c_write(DS1307_ADD);
	i2c_write(0);
	i2c_write(decToBcd(second));
	i2c_write(decToBcd(minute));
	i2c_write(decToBcd(hour));
	i2c_write(decToBcd(dayOfWeek));
	i2c_write(decToBcd(dayOfMonth));
	i2c_write(decToBcd(month));
	i2c_write(decToBcd(year));
	i2c_stop();

}

void getDateDs1307(int *second, int *minute, int *hour, int *dayOfWeek,
		int *dayOfMonth, int *month, int *year) {
	i2c_start();
	i2c_write(DS1307_ADD);
	i2c_write(0);
	i2c_start();
	i2c_write(DS1307_ADD + 1);
	*second = i2c_read();
	*minute = i2c_read();
	*hour = i2c_read();
	*dayOfWeek = i2c_read();
	*dayOfMonth = i2c_read();
	*month = i2c_read();
	*year = i2c_read(0);
	i2c_stop();
}

int main(void) {
	int second, minute, hour, dayOfWeek, dayOfMonth, month, year;

	second = 45;
	minute = 3;
	hour = 7;
	dayOfWeek = 5;
	dayOfMonth = 17;
	month = 4;
	year = 8;
	setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);

	while (TRUE) {
		int second, minute, hour, dayOfWeek, dayOfMonth, month, year;

		getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
				&year);
		printf("%2d:%2d:%2d", hour, minute, second);

		delay_ms(1000);
	}
	return 0;
}
