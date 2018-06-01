//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>	// needed for ftruncate()
#include <fcntl.h>	//For O_* constants
#include <pthread.h>
#include <string.h>

sem_t semaphore;	//Semaphore to confirm consumption only when somehting is produced

void *runner1(void *param) {	//created producer thread will call this function

	sprintf(param, "Procucer thread writes frist");	// write to the shared memory object
	printf("Producer thread writes first\n");
	sem_post(&semaphore);

	pthread_exit(0);
}

void *runner2(void *param) {	//created consumer thread will call this function

	sem_wait(&semaphore);
	printf("Consumer thread read: %s\n",(char *)param);	// read from the shared memory object
	sprintf(param, "First Update");	// write to the shared memory object
	printf("Consumer Updated: First Update\n");
	sem_post(&semaphore);

	pthread_exit(0);
}

int main() {
	
	/* Initialize the semaphore to default attributes
	* Share flag 0 to indicate only current process share the semaphore
	* And initialize semaphore to 0 to denote no value is written to synchronize read and write */
	if(sem_init(&semaphore, 0, 0) != 0) return 1;

	const int SIZE = 4096;	// the size (in bytes) of shared memory object
	const char* name = "OS";	// name of the shared memory object
	int shm_fd;	// shared memory file descriptor
	void *ptr;	// pointer to shared memory obect

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);	//create the shared memory object
	ftruncate(shm_fd, SIZE);	// configure the size of the shared memory object
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);	// memory map the shared memory object

	pthread_t tid1, tid2;	// thread identifiers
	pthread_attr_t attr;	//set of thread attributes
	pthread_attr_init(&attr);	//initialize attr to the default attributes

	// create threads; they will activate respective runner() by default
	pthread_create(&tid1, &attr, runner1, ptr);	// producer process: create a thread; it will activate runner1() by default
	pthread_create(&tid2, &attr, runner2, ptr);	// consumer process: create a thread; it will activate runner2() by default

	pthread_join(tid1, NULL);	// wait for the producer thread to exit
	pthread_join(tid2, NULL);	// wait for the consumer thread to exit
	shm_unlink(name);	//remove the shared memory object
}

/* Both producer and consumer thread might want to access the same memory at the same time.
* there fore mutex is necessary whick is ensured by the semaphore
* Deadlock condition would not result since there will be only two threads and mutex is in effect
* semaphor is necessary to make sure there is at least a value written for consumer to consume
* And to make sure both the thread do not write at the same time at the same place. */
