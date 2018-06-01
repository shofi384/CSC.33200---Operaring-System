//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <stdio.h>
#include <fcntl.h>	//for file manipulation
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main() {
	int read_fd, write_fd1, write_fd2, closeSuccess;
	
	read_fd = open("source.txt", O_RDONLY);
	if(read_fd == -1) {
		printf("\n source.txt open() failed with error [%s]\n\n", strerror(errno));
	return 1;
	}
	write_fd1 = open("destination1.txt", O_WRONLY);
	if(write_fd1 == -1) {
		printf("\n destination1.txt open() failed with error [%s]\n\n", strerror(errno));
	return 1;
	}
	write_fd2 = open("destination2.txt", O_WRONLY);
	if(write_fd2 == -1) {
		printf("\n destination2.txt open() failed with error [%s]\n\n", strerror(errno));
	return 1;
	}

	//attempt to read the opened file and copy the read buffer to destination files
	size_t read_bytes, write_bytes, buffer_size=100;
	char buffer[buffer_size];
	while((read_bytes = read (read_fd, &buffer, buffer_size)) > 0) {
		for(int i=0; i<buffer_size; i++)	//replacing each '1' with 'A'
			if(buffer[i]=='1') buffer[i] = 'A';
		write_bytes = write(write_fd1, &buffer, read_bytes); //writing the 100bytes/char buffer to destination1.txt file
		if(read_bytes != write_bytes) {	//check if written correctly at every iteration
			perror("Error: ");
			return 1;
		}
		if((read_bytes = read (read_fd, &buffer, 50))>0) {
			for(int i=0; i<50; i++) //replacing each '2' with 'B'
				if(buffer[i]=='2') buffer[i] = 'B';
			write_bytes = write(write_fd2, &buffer, read_bytes); //writing the 50bytes/char buffer to destination2.txt file
			if(read_bytes != write_bytes) {	//check if written correctly at every iteration
				perror("Error: ");
				return 1;
			}
		}
	}

	if(read_bytes == -1) {	//checking if read() went wrong
		printf(" read() and write() unsuccessful\n\n");
		return 1;
	}
	
	closeSuccess = close(read_fd);
	if(closeSuccess == -1) {
		printf("\n source.txt close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	closeSuccess = close(write_fd1);
	if(closeSuccess == -1) {
		printf("\n destination1.txt close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	closeSuccess = close(write_fd2);
	if(closeSuccess == -1) {
		printf("\n destination2.txt close() failed with error [%s]\n", strerror(errno));
	return 1;
	}
	printf(" \nWrite completed to both destination files.\n\n");

	return 0;
}
