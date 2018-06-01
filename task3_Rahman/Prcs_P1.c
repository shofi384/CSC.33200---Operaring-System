//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <stdio.h>
#include <fcntl.h>	//for file manipulation
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("\n Usage: please provide two file names to be created\n");
		return 1;
	}

	int write_fd, closeSuccess;

	write_fd = creat(argv[1], S_IRWXO | S_IRWXU | S_IRWXG);
	if(write_fd == -1)
		printf("\n %s creat() failed with error [%s]\n\n", argv[1], strerror(errno));
	closeSuccess = close(write_fd);
	if(closeSuccess == -1)
		printf("\n %s created but close() failed with error [%s]\n", argv[1], strerror(errno));

	write_fd = creat(argv[2], S_IRWXO | S_IRWXU | S_IRWXG);
	if(write_fd == -1)
		printf("\n %s creat() failed with error [%s]\n\n", argv[2], strerror(errno));
	closeSuccess = close(write_fd);
	if(closeSuccess == -1)
		printf("\n %s created but close() failed with error [%s]\n", argv[2], strerror(errno));

	printf(" \n %s and %s creat() successful \n\n", argv[1], argv[2]);
	return 0;
}
