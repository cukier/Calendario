/*
 * calendario.h
 *
 *  Created on: 02/02/2012
 *      Author: cuki
 */

#ifndef CALENDARIO_H_
#define CALENDARIO_H_

struct TIMERKEEPER_REG {
	int seconds;

}tempo;

void atualizaTimeKeeperRegisters(void) {

	register int cont;
	register int aux;

	for (cont = 0x00; cont <= 0x07; ++cont) {
		aux = read_i2c(cont);
		switch (aux) {
		case 0x00:
			tempo.seconds = aux;
			break;
		case 0x01

		}

	}
}

#endif /* CALENDARIO_H_ */
