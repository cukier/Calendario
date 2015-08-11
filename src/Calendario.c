/*
 * Calendario.c
 *
 *  Created on: 11/08/2015
 *      Author: cuki
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serial.h"

int main(int argc, char **argv) {

	time_t rawtime;
	struct tm * timeinfo;
	int fd, cont;
	char porta[30];

	if (argc == 1) {
		fprintf(stderr, "Usage: comando nome_da_porta");
		return (EXIT_FAILURE);
	}

	strcpy(porta, argv[1]);

	fd = open_port(porta);
	set_port(19200, fd);
	write(fd, "\f", 2);

	for (cont = 0; cont < 3; ++cont) {
		usleep(1000000);
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		printf("Current local time and date: %s", asctime(timeinfo));
		write(fd, asctime(timeinfo), 16);
		write(fd, "\n", 1);
	}

	close(fd);

	return EXIT_SUCCESS;
}
