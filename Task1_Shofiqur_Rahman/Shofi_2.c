#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int read_fd, write_fd;
	
	if(argc != 3) {
		printf("\n Usage: please provide the file to open\n\tand the destination file name\n");
		return 1;
	}

	//attempt to open a file given in the argument
	read_fd = open(argv[1], O_RDONLY);
	if(read_fd == -1) {
		printf("\n open() failed with error [%s]\n\n", strerror(errno));
	return 1;
	}
	else printf("\n open() Successful\n\n");

	//attempt to create file to write to
	write_fd = creat(argv[2], O_RDWR);
	if(write_fd == -1) {
		printf("\n creat() failed with error [%s]\n\n", strerror(errno));
	return 1;
	}
	else printf(" creat() Successful\n\n");

	//attempt to read the opened file
	size_t read_bytes, write_bytes;
	char buffer;
	while((read_bytes = read (read_fd, &buffer, 1)) > 0) {
		write_bytes = write(write_fd, &buffer, read_bytes);
		if(read_bytes != write_bytes)
			perror("Error: ");
	}
	if(read_bytes == -1) {
		printf("\n read() failed with error [%s]\n", strerror(errno));
		return 1;
	}
	else if(read_bytes == 0)
		printf(" read() and write() Successful\n\n");

	//attempt to close the read and write files
	int closeSuccess;
	closeSuccess = close(read_fd);
	if(closeSuccess == -1) {
		printf("\n read file close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	closeSuccess = close(write_fd);
	if(closeSuccess == -1) {
		printf("\n write file close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	else printf(" close() Successful \n File copied successfully to %s\n\n", argv[2]);

	return 0;
}
