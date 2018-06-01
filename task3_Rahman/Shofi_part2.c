//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <unistd.h>	//include process functionalities
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>	//include exit functionalities

int main() {
	pid_t pid = fork();
	if(pid == -1) {	//check if the forking went wrong
		perror("fork() failure: ");
		exit(EXIT_FAILURE);
	}
	
	else if(pid == 0) {	//if current process is the child process
		printf("\n I am the child process, my PID: %d and my parent PID: %d \n",getpid(), getppid());
		char* const arg[] = {"ls", "-la", "/bin", NULL};
		printf(" Details about %s: \n", arg[2]);
		execvp("/bin/ls", arg);
		printf("EXECVP Failed\n");
	}

	else {	//if the current process is the parent process
		int status = -1;
		waitpid(pid, &status, 0);	//terminate child process
		printf("\n Child process exited with return code %d\n\n", status);
	}

	exit(EXIT_SUCCESS);
}
