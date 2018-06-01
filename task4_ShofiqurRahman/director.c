//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <stdio.h>	//for I/O
#include <stdlib.h>	//for random int, exit functionalities
#include <fcntl.h>	//for file manipulation
#include <string.h>	//for strerror
#include <errno.h>	//errno
#include <sys/wait.h>	//for waitpid function
#include <unistd.h>	//for read/write

int main(int argc, char *argv[]) {
	pid_t workerPid, pid[5];
	int number, status, closeSuccess;
	int gradeArray[10][10];

	FILE* readFile = fopen("grades.txt", "r");	//Open file for reading
	FILE* writeFile = fopen("avg.txt", "w+");	//Truncate to zero length or create file for update(read and write)
	FILE* logFile = fopen("log.txt", "w+");		//To log activities

	if(readFile == NULL || writeFile == NULL) {
		printf("\n fopen() failed with error [%s]\n", strerror(errno));
		fprintf(logFile, "\n fopen() failed with error [%s]\n", strerror(errno));
		return 1;
	}

	for(int i = 0; i<10; i++)	//read all grades to a 2d array
		for(int j = 0; j<10; j++)
			if(fscanf(readFile, "%d", &number) != EOF)
				gradeArray[i][j] = number;

	closeSuccess = fclose(readFile);	//close the grades.txt
	if(closeSuccess != 0) {
		printf("\n grades.txt fclose() failed with error [%s]\n", strerror(errno));
		fprintf(logFile, "\n grades.txt fclose() failed with error [%s]\n", strerror(errno));
		return 1;
	}

	for(int i = 0; i<10; i++) {
		for(int j = 0; j<10; j++)
			printf("%5d", gradeArray[i][j]);
		printf("\n");
	}

	pid[0] = fork();
	if(pid[0] < 0) {	//check if the forking went wrong
		perror("Manager1 fork() failure: ");
		fprintf(logFile, "Manager1 fork() failure");
		return 1;
	}
	else if(pid[0] == 0) {	//if current process is the manager1 process
		printf("\nI am the manager1 process with pid: %d and ppid: %d\n", getpid(), getppid());
		fprintf(logFile, "\nI am the manager1 process with pid: %d and ppid: %d\n", getpid(), getppid());
		workerPid = fork(); //create worker1

		if(workerPid == -1) {	//check if the forking went wrong
			perror("worker1 fork() failure: ");
			fprintf(logFile, "worker1 fork() failure");
			exit(EXIT_FAILURE);
		}

		else if(workerPid == 0) {	//if current process is the worker1
			printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
			fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());
			float total = 0;
			for(int i=0; i<10; i++)
				total += gradeArray[0][i];
			fseek(writeFile, 10, SEEK_SET );
			fprintf(writeFile, "%10.2f", total/10);
		}

		else {	//if current process is the manager1 process, terminate worker1
			waitpid(workerPid, &status, 0);	//terminate worker1 process
			workerPid = fork();	//create worker2
			if(workerPid == -1) {	//check if the forking went wrong
				perror("worker2 fork() failure: ");
				fprintf(logFile, "worker2 fork() failure");
				exit(EXIT_FAILURE);
			}
			else if(workerPid == 0) {	//if current process is the worker2
				printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
				fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());
				float total = 0;
				for(int i=0; i<10; i++)
					total += gradeArray[1][i];
				fseek(writeFile, 20, SEEK_SET );
				fprintf(writeFile, "%10.2f", total/10);
			}
			else {	//if current process is still the manager1 process, terminate worker2
				waitpid(workerPid, &status, 0);	//terminating worker2
			}
		}
	}

	else {	//if the current process is still the director process
		pid[1] = fork();
		if(pid[1] < 0) {	//check if the forking went wrong
			perror("Manager2 fork() failure: ");
			fprintf(logFile, "manager2 fork() failure");
			return 1;
		}
		else if(pid[1] == 0) {	//if current process is the manager2 process
			printf("\nI am the manager2 process with pid: %d and ppid: %d\n", getpid(), getppid());
			fprintf(logFile, "\nI am the manager2 process with pid: %d and ppid: %d\n", getpid(), getppid());
			workerPid = fork(); //create worker1

			if(workerPid == -1) {	//check if the forking went wrong
				perror("worker1 fork() failure: ");
				exit(EXIT_FAILURE);
			}

			else if(workerPid == 0) {	//if current process is the worker1
				printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
				fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());
				float total = 0;
				for(int i=0; i<10; i++)
					total += gradeArray[2][i];
				fseek(writeFile, 30, SEEK_SET );
				fprintf(writeFile, "%10.2f", total/10);
			}

			else {	//if current process is the manager2 process, terminate worker1
				waitpid(workerPid, &status, 0);	//terminate worker1 process
				workerPid = fork();	//create worker2
				if(workerPid == -1) {	//check if the forking went wrong
					perror("worker2 fork() failure: ");
					exit(EXIT_FAILURE);
				}
				else if(workerPid == 0) {	//if current process is the worker2
					printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
					fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());
					float total = 0;
					for(int i=0; i<10; i++)
						total += gradeArray[3][i];
					fseek(writeFile, 40, SEEK_SET);
					fprintf(writeFile, "%10.2f", total/10);
				}
				else {	//if current process is still the manager2 process, terminate worker2
					waitpid(workerPid, &status, 0);	//terminating worker2
				}
			}
		}

		else {	//if the current process is still the director process
			pid[2] = fork();
			if(pid[2] < 0) {	//check if the forking went wrong
				perror("Manager3 fork() failure: ");
				return 1;
			}
			else if(pid[2] == 0) {	//if current process is the manager3 process
				printf("\nI am the manager3 process with pid: %d and ppid: %d\n", getpid(), getppid());
				fprintf(logFile, "\nI am the manager3 process with pid: %d and ppid: %d\n", getpid(), getppid());
				workerPid = fork(); //create worker1

				if(workerPid == -1) {	//check if the forking went wrong
					perror("worker1 fork() failure: ");
					exit(EXIT_FAILURE);
				}

				else if(workerPid == 0) {	//if current process is the worker1
					printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
					fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());

					float total = 0;
					for(int i=0; i<10; i++)
						total += gradeArray[4][i];
					fseek(writeFile, 50, SEEK_SET );
					fprintf(writeFile, "%10.2f", total/10);
				}

				else {	//if current process is the manager3 process, terminate worker1
					waitpid(workerPid, &status, 0);	//terminate worker1 process
					workerPid = fork();	//create worker2

					if(workerPid == -1) {	//check if the forking went wrong
						perror("worker2 fork() failure: ");
						exit(EXIT_FAILURE);
					}
					else if(workerPid == 0) {	//if current process is the worker2
						printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
						fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());

						float total = 0;
						for(int i=0; i<10; i++)
							total += gradeArray[5][i];
						fseek(writeFile, 60, SEEK_SET );
						fprintf(writeFile, "%10.2f", total/10);
					}
					else {	//if current process is still the manager3 process, terminate worker2
						waitpid(workerPid, &status, 0);	//terminating worker2
					}
				}
			}

			else {	//if the current process is still the director process
				pid[3] = fork();
				if(pid[3] < 0) {	//check if the forking went wrong
					perror("Manager4 fork() failure: ");
					return 1;
				}
				else if(pid[3] == 0) {	//if current process is the manager4 process
					printf("\nI am the manager4 process with pid: %d and ppid: %d\n", getpid(), getppid());
					fprintf(logFile, "\nI am the manager4 process with pid: %d and ppid: %d\n", getpid(), getppid());
					workerPid = fork(); //create worker1

					if(workerPid == -1) {	//check if the forking went wrong
						perror("worker1 fork() failure: ");
						exit(EXIT_FAILURE);
					}

					else if(workerPid == 0) {	//if current process is the worker1
						printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
						fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());

						float total = 0;
						for(int i=0; i<10; i++)
							total += gradeArray[6][i];
						fseek(writeFile, 70, SEEK_SET );
						fprintf(writeFile, "%10.2f", total/10);
					}

					else {	//if current process is the manager4 process, terminate worker1
						waitpid(workerPid, &status, 0);	//terminate worker1 process

						workerPid = fork();	//create worker2
						if(workerPid == -1) {	//check if the forking went wrong
							perror("worker2 fork() failure: ");
							exit(EXIT_FAILURE);
						}
						else if(workerPid == 0) {	//if current process is the worker2
							printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
							fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());

							float total = 0;
							for(int i=0; i<10; i++)
								total += gradeArray[7][i];
							fseek(writeFile, 80, SEEK_SET );
							fprintf(writeFile, "%10.2f", total/10);
						}
						else {	//if current process is still the manager4 process, terminate worker2
							waitpid(workerPid, &status, 0);	//terminating worker2
						}
					}
				}

				else {	//if the current process is still the director process
					pid[4] = fork();
					if(pid[4] < 0) {	//check if the forking went wrong
						perror("Manager5 fork() failure: ");
						return 1;
					}
					else if(pid[4] == 0) {	//if current process is the worker5 process
						printf("\nI am the manager5 process with pid: %d and ppid: %d\n", getpid(), getppid());
						fprintf(logFile, "\nI am the manager5 process with pid: %d and ppid: %d\n", getpid(), getppid());
						workerPid = fork(); //create worker1

						if(workerPid == -1) {	//check if the forking went wrong
							perror("worker1 fork() failure: ");
							exit(EXIT_FAILURE);
						}

						else if(workerPid == 0) {	//if current process is the worker1
							printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
							fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());

							float total = 0;
							for(int i=0; i<10; i++)
								total += gradeArray[8][i];
							fseek(writeFile, 90, SEEK_SET );
							fprintf(writeFile, "%10.2f", total/10);
						}

						else {	//if current process is the manager5 process, terminate worker1
							waitpid(workerPid, &status, 0);	//terminate worker1 process
							workerPid = fork();	//create worker2

							if(workerPid == -1) {	//check if the forking went wrong
								perror("worker2 fork() failure: ");
								exit(EXIT_FAILURE);
							}
							else if(workerPid == 0) {	//if current process is the worker2
								printf("\nI am the process calculating average with pid: %d and ppid: %d\n", getpid(), getppid());
								fprintf(logFile, "\nI am the process calculating avarage with pid: %d and ppid: %d\n", getpid(), getppid());
								float total = 0;
								for(int i=0; i<10; i++)
									total += gradeArray[9][i];
								fseek(writeFile, 100, SEEK_SET );
								fprintf(writeFile, "%10.2f", total/10);
							}
							else {	//if current process is still the manager5 process, terminate worker2
								waitpid(workerPid, &status, 0);	//terminating worker2
							}
						}
					}

					else {	//if the current process is still the director process
						for(int i=0; i<5; i++) {
							waitpid(pid[i], &status, 0);	//terminate manager processes
							printf("\n manager %d process exited with return code %d\n", i+1, status);
						}
						printf("\n");

						float average;
						fseek(writeFile, 10, SEEK_SET );
						for(int i = 0; i<10; i++)
							if(fscanf(writeFile, "%f", &average) != EOF)
								printf("Average from Chapter # %d Homework # %d: %10.2f\n", i/5+1, i%2+1, average);

						closeSuccess = fclose(writeFile);	//close the avg.txt
						if(closeSuccess != 0) {
							printf("\n avg.txt fclose() failed with error [%s]\n", strerror(errno));
							fprintf(logFile, "\n avg.txt fclose() failed with error [%s]\n", strerror(errno));
							return 1;
						}

						closeSuccess = fclose(logFile);	//close the log.txt
						if(closeSuccess != 0) {
							printf("\n log.txt fclose() failed with error [%s]\n", strerror(errno));
							fprintf(logFile, "\n avg.txt fclose() failed with error [%s]\n", strerror(errno));
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}
