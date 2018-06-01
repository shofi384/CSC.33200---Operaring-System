//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <unistd.h>	//include process functionalities
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>	//include exit functionalities

int main(int argc, char *argv[]) {
	//parent process P
	int a = 10, b = 25, fq = 0, fr = 0, status = -1;
	
	fq = fork();	//fork the first child - call it process Q
	if(fq == -1) { //check if the forking went wrong
                perror("fork(Q) failure: ");
                exit(EXIT_FAILURE);
	}
	if(fq == 0) {	//if child Q successfully forked and the current process is Q
		a = a + b;
		printf("\nThe value of a: %d\nThe value of b: %d\nAnd the pid(Q): %d\n", a, b, getpid());
		
		fr = fork();	//fork another child - child of the running child - call it process R
		if(fr == -1) { //check if the forking went wrong
                	perror("fork(R) failure: ");
                	exit(EXIT_FAILURE);
		}
		if(fr != 0) {	//if the process is not R, but Q
			b = b + 20;
			printf("\nThe value of a: %d\nThe value of b: %d\nAnd the pid(Q): %d\n", a, b, getpid());
			waitpid(fr, &status, 0);	//terminating R
		}
		else {	//if child R successfully forked and current process is R
			a = (a * b) + 30;
			printf("\nThe value of a: %d\nThe value of b: %d\nAnd the pid(R): %d\n", a, b, getpid());
		}
	}

	else {	//if the current process is still the parent process P
		b = a + b - 5;
		printf("\nThe value of a: %d\nThe value of b: %d\nAnd the pid(P): %d\n", a, b, getpid());
		waitpid(fq, &status, 0);	//terminating Q
	}
	exit(EXIT_SUCCESS);
}
