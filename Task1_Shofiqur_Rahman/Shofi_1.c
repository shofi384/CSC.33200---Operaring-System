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

	//attempt to open a file given in the argument
	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		printf("\n open() failed with error [%s]\n\n", strerror(errno));
	return 1;
	}
	else printf("\n open() Successful\n\n");

	//attempt to read the opened file
	size_t bytes;
	char buffer;
	while((bytes = read (fd, &buffer, 1)) > 0)
		printf("%c", buffer);
	if(bytes == -1) {
		printf("\n read() failed with error [%s]\n", strerror(errno));
		return 1;
	}
	else if(bytes == 0)
		printf("\n\n read() Successful: reached end of file!\n");

	//attempt to close the read file
	int closeSuccess;
	closeSuccess = close(fd);
	if(closeSuccess == -1) {
		printf("\n close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	else printf("\n close() Successful\n\n");

	return 0;
}
