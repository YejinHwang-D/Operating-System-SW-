/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32195044
*	    Student name : Hwang Yejin
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
	int i, j, n=0, temp=0, pro_count;
	process proc[SIZE];

	printf("    Workload Insert    \n");
	printf("Number of Process : ");
	while (1) {
		scanf("%d", &temp);
		if (temp > 0)
			break;
		else {
			printf("please insert again...\n");
			rewind(stdin);
		}
	}
	n = temp;
	pro_count = n;
	
	printf("Process Info \n");
	for (i=0; i<n; i++) {
		printf("[  %d. Process  ]\n", i+1);
		proc[i].id = i+1;
		printf("--> Arrive time : ");
		while (1) {
			scanf("%d", &temp);
			if (temp >=  0)
				break;
			else {
				printf("please insert again...\n");
				rewind(stdin);
			}
		}
		proc[i].arr_time = temp;
		printf("--> Cpu time : ");
		while (1) {
			scanf("%d", &temp);
			if (temp > 0)
				break;
			else {
				printf("please insert again...\n");
				rewind(stdin);
			}

		}
		proc[i].cpu_time = temp;
	}
	printf("========== FIFO ==========\n");
	fifo_func(proc, n);


	printf("========== SJF ===========\n");
	sjf_func(proc, n);


	printf("======== RR (q=1) ========\n");
	rr_func(proc, n, 1);


	printf("======== RR (q=4) ========\n");
	rr_func(proc, n, 4);


	printf("============ MLFQ (q=1) ===============\n");
	mlfq_func(proc, n, 1);

	printf("========== MLFQ (q=2^i) =============\n");
	mlfq_func(proc, n, 2);

	return 0;
}

