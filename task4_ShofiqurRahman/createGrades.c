//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <stdio.h>	//for I/O, FILE stream
#include <stdlib.h>	//for random int
#include <string.h>	//for strerror
#include <errno.h>	//errno

int main() {
	int closeSuccess, grade, students = 10, chapter = 2, hw = 5;
	
	//create the grade file that will contain all the students grades
	FILE* file = fopen("grades.txt", "w");	//flag 'w' truncate file to zero length or create new file for writing
	if(file == NULL) {
		printf("\n grades.txt creation failed with error [%s]\n\n", strerror(errno));
		return 1;
	}

	for(int i = 0; i<students; i++) {	//populate grades.txt file with mock grades
		for(int j = 0; j<chapter*hw; j++) {
			grade = rand()%20;
			fprintf(file, "%5d", grade);
		}
		fprintf(file, "\n");
	}

	closeSuccess = fclose(file);
	if(closeSuccess != 0) {
		printf("\n grades.txt close() failed with error [%s]\n", strerror(errno));
		return 1;
	}

	printf("\n\tGrades writing successful!\n\n");
	return 0;
}
