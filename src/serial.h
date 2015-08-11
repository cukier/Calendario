/*
 * serial.h
 *
 *  Created on: 07/07/2015
 *      Author: cuki
 */

#ifndef SERIAL_H_
#define SERIAL_H_

extern int open_port(char *porta);
extern int set_port(int baud_rate, int fd);

#endif /* SERIAL_H_ */
