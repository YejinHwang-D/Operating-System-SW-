/*
*   DKU Operating System Lab
*           Lab2 (Vehicle production Problem)
*           Student id : 32195044
*           Student name : Hwang Yejin
*
*   lab2_sync.c :
*       - lab2 main file.
*       - must contains Vehicle production Problem function's declations.
*
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int balance = 0;
int loops;
int pro_loops;
int con_loops;

void lab2_sync_usage(char *cmd) {
	printf("\n Usage for %s : \n",cmd);
    printf("    -c: Total number of vehicles produced, must be bigger than 0 ( e.g. 100 )\n");
    printf("    -q: RoundRobin Time Quantum, must be bigger than 0 ( e.g. 1, 4 ) \n");
}

void lab2_sync_example(char *cmd) {
	printf("\n Example : \n");
    printf("    #sudo %s -c=100 -q=1 \n", cmd);
    printf("    #sudo %s -c=10000 -q=4 \n", cmd);
}


void put(int value) {		//enqueue
	buffer[fill_ptr] = value;
	fill_ptr = (fill_ptr + 1) % MAX;
	balance++;
}

int get() {	//dequeue
	int tmp = buffer[use_ptr];
	use_ptr = (use_ptr+1)%MAX;
	balance--;
	return tmp;
}

pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void *producer(void *arg) {
	int i;

 	while (1) {
		if (pro_loops <= 0 ) {
			break;
		}
		pthread_mutex_lock(&mutex);
		while (balance == MAX)
			pthread_cond_wait(&empty, &mutex);
		put(loops-pro_loops);
		pro_loops--;
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
	}
}

void *consumer(void *arg) {
	int i;
	int tmp = 0;	//init

	while (1) {
		if (con_loops <= 0) {
			break;
		}
		pthread_mutex_lock(&mutex);
		while (balance == 0)
			pthread_cond_wait(&fill, &mutex);
		tmp = get();
		con_loops--;
		//printf("Consumer consume : %d\n", tmp);
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
	}
}


int main(int argc, char* argv[]) {
	char op;
	int n; char junk;
	int time_quantum = 0;
	int total_car = 0;
	double exe_time = 0;
	struct timeval stime, etime, gap;
	int T_counts = 1;
	
	if (argc <= 1) {
		lab2_sync_usage(argv[0]);
		lab2_sync_example(argv[0]);
		exit(0);
	}
	for (int i = 1; i < argc; i++) {
		if (sscanf(argv[i], "-c=%d%c", &n, &junk) == 1) {
			total_car = n;
			loops = con_loops = pro_loops = total_car;
		}
		else if (sscanf(argv[i], "-q=%d%c", &n, &junk) == 1) {
			time_quantum = n; //이전 함수에서 썼던 대기 시간 init	
		}
		else {			//error
			lab2_sync_usage(argv[0]);
			lab2_sync_example(argv[0]);
			exit(0);
		}
	}

	gettimeofday(&stime, NULL);
	pthread_t pro[1], con[1];
	for (int i=0; i<T_counts; i++) {
		pthread_create(&pro[i], NULL, producer, (void*)buffer);
		pthread_create(&con[i], NULL, consumer, (void*)buffer);
	}

	for (int i=0; i<T_counts; i++) {
		pthread_join(pro[i], NULL);
		pthread_join(con[i], NULL);
	}
	gettimeofday(&etime, NULL);

	printf("==== Producer/Consumer (1) ====\n");
	//Total Produce Number
	printf("\tTotal Produce Number = %d\n", total_car);
	printf("\tTotal Number of Threads = %d\n", T_counts);

	//Final Balance Value
	printf("\tFinal Balance Value = %d\n", balance);
	
	//check exe_time
	gap.tv_sec = etime.tv_sec - stime.tv_sec;
	gap.tv_usec = etime.tv_usec - stime.tv_usec;
	if (gap.tv_usec < 0) {
		gap.tv_sec = gap.tv_sec - 1;
		gap.tv_usec = gap.tv_usec + 1000000;
	}
	printf("\tExecution time :  %ldsec : %liusec\n", gap.tv_sec, gap.tv_usec);
	
	


	return 0;
}
