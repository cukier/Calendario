/*
 * main.c
 *
 *  Created on: Aug 1, 2015
 *      Author: cuki
 */

#include <src/serial.c>
#include <stdio.h>      /* puts, printf */
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <unistd.h>

int get_msg_size(char *msg) {
	int cont = 0;

	while (*msg != '\0') {
		cont++;
		msg++;
	}

	return cont;
}

char *get_curr_time() {
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	return asctime(timeinfo);
}

int main(int argc, char **argv) {
	int fd, size, cont = 10, i;
	char *msg;

	if (argc == 1) {
		printf("No door?\n");
		exit(EXIT_FAILURE);
	}

	fd = open_port(argv[1]);
	set_port(9600, fd);

	for (i = 0; i < cont; ++i) {
		msg = get_curr_time();
		size = get_msg_size(msg);
		write(fd, msg, size);
		usleep(1000000);
	}

	close(fd);

	return 0;
}
