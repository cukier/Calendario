#include <18F252.h>

#fuses H4
#use delay(crystal=8MHz, clock=32MHz)
#use rs232(baud=9660, xmit=PIN_C4)
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

// Gets the date and time from the ds1307
void getDateDs1307(int *second, int *minute, int *hour, int *dayOfWeek,
		int *dayOfMonth, int *month, int *year) {
	// Reset the register pointer
	Wire.beginTransmission(DS1307_I2C_ADDRESS);
	Wire.send(0);
	Wire.endTransmission();

	Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

	// A few of these need masks because certain bits are control bits
	*second = bcdToDec(Wire.receive() & 0x7f);
	*minute = bcdToDec(Wire.receive());
	*hour = bcdToDec(Wire.receive() & 0x3f); // Need to change this if 12 hour am/pm
	*dayOfWeek = bcdToDec(Wire.receive());
	*dayOfMonth = bcdToDec(Wire.receive());
	*month = bcdToDec(Wire.receive());
	*year = bcdToDec(Wire.receive());
}

void setup() {
	int second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	Wire.begin();
	Serial.begin(9600);

	// Change these values to what you want to set your clock to.
	// You probably only want to set your clock once and then remove
	// the setDateDs1307 call.
	second = 45;
	minute = 3;
	hour = 7;
	dayOfWeek = 5;
	dayOfMonth = 17;
	month = 4;
	year = 8;
	setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void loop() {
	int second, minute, hour, dayOfWeek, dayOfMonth, month, year;

	getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
			&year);
	Serial.print(hour, DEC);
	Serial.print(":");
	Serial.print(minute, DEC);
	Serial.print(":");
	Serial.print(second, DEC);
	Serial.print("  ");
	Serial.print(month, DEC);
	Serial.print("/");
	Serial.print(dayOfMonth, DEC);
	Serial.print("/");
	Serial.print(year, DEC);
	Serial.print("  Day_of_week:");
	Serial.println(dayOfWeek, DEC);

	delay(1000);
}
