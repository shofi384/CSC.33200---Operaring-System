//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <unistd.h>	//include process functionalities
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>	//include exit functionalities

int main(int argc, char *argv[]) {
	pid_t pid;
	int status = -1;

	printf("\n I am the parent and my pid is: %d\n",getpid());

	pid = fork(); //create child1

	if(pid == -1) {	//check if the forking went wrong
		perror("fork1() failure: ");
		exit(EXIT_FAILURE);
	}

	else if(pid == 0) {	//if current process is the child1
		printf("\n I am child one, my pid is: %d and my parent pid is: %d\n",getpid(), getppid());
	}

	else {	//if current process is the parent, terminate child one
		waitpid(pid, &status, 0);
		printf(" child one exited with return code %d\n\n", status);
		
		pid = fork(); //create child2
		if(pid == -1) {	//check if the forking went wrong
			perror("fork2() failure: ");
			exit(EXIT_FAILURE);
		}
		else if(pid == 0) {	//if current process is the child two
			printf(" I am child two my pid is: %d and the parent pid is: %d\n",getpid(), getppid());
		}
		else {	//if it is the parent process, terminate child two
			status = -1;
			waitpid(pid, &status, 0);	//terminating child2
			printf(" child two exited with return code %d\n\n", status);
		}
	}

	exit(EXIT_SUCCESS);
}
