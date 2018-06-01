#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int fd;
	if(argc != 2) {
		printf("\n Usage: please provide the file to open\n");
		return 1;
	}
	errno = 0;

	//attempt to open a file or create if not present
	fd = open(argv[1], O_RDONLY | O_CREAT);
	if(fd == -1) {
		printf("\n open() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	else printf("\n open() Successful\n");

	//attempt to read the opened file
	size_t bytes = 10;
	char buffer[bytes];
	bytes = read(fd, buffer, bytes);
	if(bytes == -1) {
		printf("\n read() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	else printf("\n read() Successful: %zd bytes have been read\n", bytes);

	//attempt to close the read file
	int closeSuccess;
	closeSuccess = close(fd);
	if(closeSuccess == -1) {
		printf("\n close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	else printf("\n close() Successful\n");

	return 0;
}
