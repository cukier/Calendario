#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

int open_port(char *porta) {
	int fd;

	fd = open(porta, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1) {
		fprintf(stderr, "Problemas ao abrir a porta \"%s\" : %s\n", porta,
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	return fd;
}

int set_port(int baud_rate, int fd) {
	struct termios options;
	speed_t speed;

	switch (baud_rate) {
	default:
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	}

	tcgetattr(fd, &options);
	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CRTSCTS;

	tcsetattr(fd, TCSANOW, &options);

	return 0;
}
