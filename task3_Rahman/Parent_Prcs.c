//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <unistd.h>	//include process functionalities
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>	//include exit functionalities

int main() {
	int status;
	pid_t pid = fork();
	if(pid == -1) {	//check if the forking went wrong
		perror("child1 fork() failure: ");
		exit(EXIT_FAILURE);
	}
	
	else if(pid == 0) {	//if current process is the child1 process
		printf("\n I am the child1 process, my PID: %d and my parent PID: %d \n",getpid(), getppid());
		char* const arg1[] = {"Prcs_P1", "destination1.txt", "destination2.txt", NULL};
		printf(" Executing Prcs_P1. . . \n");

		sleep(3);
		execv("../task3_Rahman/Prcs_P1", arg1);
		perror("EXECV(1) Failed: ");
	}

	else {	//if the current process is the parent process
		status = -1;
		waitpid(pid, &status, 0);	//terminate child process
		printf("\n Child process1 exited with return code %d\n\n", status);
	}

	pid = fork();
	if(pid == -1) {	//check if the forking went wrong
		perror("child2 fork() failure: ");
		exit(EXIT_FAILURE);
	}
	
	else if(pid == 0) {	//if current process is the child2 process
		printf("\n I am the child2 process, my PID: %d and my parent PID: %d \n",getpid(), getppid());
		char* const arg2[] = {"Prcs_P2", NULL};
		printf(" Executing Prcs_P2. . . \n");

		sleep(3);
		execv("../task3_Rahman/Prcs_P2", arg2);
		perror("EXECV(2) Failed: ");
	}

	else {	//if the current process is the parent process
		status = -1;
		waitpid(pid, &status, 0);	//terminate child process
		printf("\n Child process2 exited with return code %d\n\n", status);
	}

	exit(EXIT_SUCCESS);
}
