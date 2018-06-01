//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <unistd.h>	//include process functionalities
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>	//include exit functionalities

int main() {
	char* date;
	pid_t pid = fork();

	if(pid == -1) {	//check if the forking went wrong
		perror("fork() failure: ");
		exit(EXIT_FAILURE);
	}
	
	else if(pid == 0) {	//if current process is the child process
		printf("\n I am the child process, my PID: %d and my parent PID: %d \n",getpid(), getppid());
		printf("\tDate & Time: \n");
		execl("/bin/date", date, NULL);
		printf("EXECL Failed\n");
	}

	else {
		int status = -1;
		waitpid(pid, &status, 0);	//terminate child process
		printf(" Child process exited with return code %d\n\n", status);
	}

	exit(EXIT_SUCCESS);
}
