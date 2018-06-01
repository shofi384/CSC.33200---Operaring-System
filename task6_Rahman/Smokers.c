//Shofiqur Rahman
//shofi384@gmail.com
//CCNY, Spring 2018

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

//four mutexes used to synchronize among four smoker processes and the agnet
pthread_mutex_t lockMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t matchMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paperMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tobaccoMutex = PTHREAD_MUTEX_INITIALIZER;

// When created, agent thread will activate this function by default
void* agentRunner() {	printf("\nAgent thread is activated. . . \n\n");	int countProduce = 0;	int maxProduceTimes = 5;	int randNumber;	while(countProduce < maxProduceTimes) {
		pthread_mutex_lock(&lockMutex);	//acquire lock to perform the following task atomically

		// random number to decide what ingredients to put on the table		randNumber = rand() % 3;
		if(randNumber == 0) {
			printf("agent is putting Tobacco and Paper on table now. . . \n");
			printf("agent signals smoker and goes to sleep now.\n");
			pthread_mutex_unlock(&matchMutex);	//signaling the smoker with match to smoke
		}
		else if(randNumber == 1) {
			printf("agent is putting Tobacco and Matches on table now. . . \n");
			printf("agent signals smoker and goes to sleep now.\n");
			pthread_mutex_unlock(&paperMutex);	//signaling the smoker with paper to smoke
		}
		else if(randNumber == 2) {
			printf("agent is putting Paper and Matches on tablenow. . . \n");
			printf("agent signals smoker and goes to sleep now.\n");
			pthread_mutex_unlock(&tobaccoMutex);	////signaling the smoker with tobacco to smoke
		}

		countProduce++;
		pthread_mutex_unlock(&lockMutex);	//atomicity completes by unlocking the lock
	}
	pthread_exit(0);
}

// When created, match smoker thread will activate this function by default
void* matchRunner(){	while(1) {
		printf("Match smoker goes to sleep.\n\n");

		//acquire lock to recieve signal from agent thread to start smoking or 
		//in other words, put agent thread to sleep
		pthread_mutex_lock(&matchMutex);
		pthread_mutex_lock(&lockMutex);	//acquire this lock to confirm atomicity

		printf("Smoker thread with match is smoking now. . . \n\tAnd awakens agent. . . \n");
		pthread_mutex_unlock(&lockMutex);	//atomicity completes by unlocking the lock
	}
	pthread_exit(0);
}

// When created, tobacco smoker thread will activate this function by default
void* tobaccoRunner() {	while(1) {
		printf("Tobacco smoker goes to sleep.\n\n");

		//acquire lock to recieve signal from agent thread to start smoking or 
		//in other words, put agent thread to sleep
		pthread_mutex_lock(&tobaccoMutex);
		pthread_mutex_lock(&lockMutex);	//acquire this lock to confirm atomicity

		printf("Smoker thread with tobacco is smoking now. . . \n\tAnd awakens agent. . . \n");
		pthread_mutex_unlock(&lockMutex);	//atomicity completes by unlocking the lock
	}
	pthread_exit(0);
}

//smoker with paper process to be called by thread
void* paperRunner() {	while(1) {
		printf("Paper smoker goes to sleep.\n\n");

		//acquire lock to recieve signal from agent thread to start smoking or 
		//in other words, put agent thread to sleep
		pthread_mutex_lock(&paperMutex);
		pthread_mutex_lock(&lockMutex);	//acquire this lock to confirm atomicity

		printf("Smoker thread with paper is smoking now. . . \n\tAnd awakens agent. . . \n");
		pthread_mutex_unlock(&lockMutex);	//atomicity completes by unlocking the lock
	}
	pthread_exit(0);
}

int main() {
	int iret_agent, iret_match, iret_tobacco, iret_paper;
	pthread_t agent, match, tobacco, paper;

	//create 4 different threads
	iret_agent = pthread_create(&agent, NULL, agentRunner, NULL);
	iret_match = pthread_create(&match, NULL, matchRunner, NULL);
	iret_tobacco = pthread_create(&tobacco, NULL, tobaccoRunner, NULL);
	iret_paper = pthread_create(&paper, NULL, paperRunner, NULL);

	//initialize each mutex of agent, match, paper, tobacco to 0
	pthread_mutex_lock(&matchMutex);
	pthread_mutex_lock(&paperMutex);
	pthread_mutex_lock(&tobaccoMutex);

	//wait for each created thread to exit
	pthread_join(agent, NULL);
	pthread_kill(match, 0);
	pthread_kill(paper, 0);
	pthread_kill(tobacco, 0);

	return 0;
}
