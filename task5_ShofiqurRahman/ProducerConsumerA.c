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

void *runner(void *param) {	//created thread will call this function
	const int SIZE = 4096;	// the size (in bytes) of shared memory object
	const char* queue = "OS";	// name of the shared memory object
	int shm_fd;	// shared memory file descriptor
	void *ptr;	// pointer to shared memory obect

	shm_fd = shm_open(queue, O_CREAT | O_RDWR, 0666);	//create the shared memory object
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);	// memory map the shared memory object

	sem_wait(&semaphore);
	printf("Consumer read: %s\n",(char *)ptr);	// read from the shared memory object
	sprintf(ptr, "First Update");	// write to the shared memory object
	printf("Consumer Updated: First Update\n");
	sem_post(&semaphore);

	pthread_exit(0);
}

int main() {

	const int SIZE = 4096;	// the size (in bytes) of shared memory object
	const char* queue = "OS";	// name of the shared memory object
	int shm_fd;	// shared memory file descriptor
	void *ptr;	// pointer to shared memory obect
	shm_fd = shm_open(queue, O_CREAT | O_RDWR, 0666);	//create the shared memory object
	ftruncate(shm_fd, SIZE);	// configure the size of the shared memory object
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);	// memory map the shared memory object

	/* Initialize the semaphore to default attributes
	* Share flag 0 to indicate only current process share the semaphore
	* And initialize semaphore to 0 to denote no value is written to synchronize read and write */
	if(sem_init(&semaphore, 0, 0) != 0) return 1;

	pthread_t tid;	//the thread identifier
	pthread_attr_t attr;	//set of thread attributes
	pthread_attr_init(&attr);	//initialize attr to the default attributes

	//Consumer process: create a thread; it will activate runner() by default
	pthread_create(&tid, &attr, runner, NULL);

	//Producer process: from main thread
	sprintf(ptr, "First Message");	// write to the shared memory object
	ptr += strlen("First Message");
	printf("Producer wrote: First Message\n");
	sem_post(&semaphore);

	pthread_join(tid, NULL);	// wait for the thread to exit
	shm_unlink(queue);	//remove the shared memory object
}

/* There will be race conditions if mutual exclusion(mutex) in not ensured.
* Both producer and consumer thread might want to access the same memory at the same time.
* mutex is ensured by the semaphore
* Deadlock condition would not result since there will be only two threads and mutex is in effect
* semaphor is necessary to make sure there is at least a value written for consumer to consume. */
