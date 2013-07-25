/*
 * i2c.h
 *
 *  Created on: 02/02/2012
 *      Author: cuki
 */

#ifndef I2C_H_
#define I2C_H_

#define SLVADDRESS 		0xD0

int handshake_i2c(int addrMemory) {

	int ack = 0;
	int cont = 100;

	do {
		i2c_start();
		ack = i2c_write(SLVADDRESS);
		i2c_write(addrMemory);
		i2c_stop();
		if (ack) {
			delay_ms(5);
			cont--;
		}
	} while (ack && cont);
	return ack;
}

int recive_i2c() {

	register int recived = 0;

	i2c_start();
	i2c_write(SLVADDRESS+1);
	recived = i2c_read(0);
	i2c_stop();

	return recived;
}

int read_i2c(int addrMemory) {

	register int recived = 0;

	if (!handshake_i2c(addrMemory)) {
		recived = recive_i2c();
		return recived;
	} else
		return 0xFF;
}

int write_i2c(int addrMemory, int info){

	int ack = 0;
	int cont = 100;

	do {
		i2c_start();
		ack = i2c_write(SLVADDRESS);
		i2c_write(addrMemory);
		i2c_write(info);
		i2c_stop();
		if (ack) {
			delay_ms(5);
			cont--;
		}
	} while (ack && cont);
	return ack;
}

#endif /* I2C_H_ */
