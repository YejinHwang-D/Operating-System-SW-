/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32195044
*	    Student name : Hwang Yejin
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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
#include <sys/stat.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

void InitQueue(Ready* r) {	//initialize Queue
	r->front = r->rear = NULL;
	r->count = 0.;
}

int isEmpty(Ready* r) {		//check Queue is free
	return r->count == 0;
}

void  Enqueue(Ready* r, int num) {	//push
	Node* now = (Node*)malloc(sizeof(Node));
	now->data = num;
	now->next = NULL;

	if (isEmpty(r)) {
		r->front = now;
	}
	else {
		r->rear->next = now;
	}
	r->rear = now; //now sched
	r->count++; //count + 1
}

int Dequeue(Ready* r) {			//pop
	int re_val = 0;
	Node* now;
	if (isEmpty(r)) {
		return re_val;
	}
	now = r->front;
	re_val = now->data;

	r->front = now->next;
	free(now);	//delete
	r->count--;	//discount Number of Queue
	return re_val;	//pop value return
}

void sched_print(int print[][SIZE], int n, int time) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<time; j++) {
			if (print[i][j] == 1)
				printf("■");
			else
				printf("□");
		}
		printf("\n");
	}
}


void fifo_func(process* pro, int n) {
	int waiting, i, j, k, print[MAX][SIZE], index, time=0;

	for (i=0; i<n; i++) {
		waiting = 0;
		
		for (j=0; j<i; j++) {
			waiting += pro[j].cpu_time;
		}
		waiting -= pro[i].arr_time;

		if (waiting <= 0) {
			pro[i].wait_time = 0;
		}
		else {
			pro[i].wait_time = waiting;
		}

		//print insert
		for (j=0; j<n; j++) {
			index = time;
			for (k=0; k<pro[i].cpu_time; k++) {
				if (j==i) {
					print[j][index] = 1;
					index++;
				}
				else {
					print[j][index] = 0;
					index++;
				}
			}
		}
		time += pro[i].cpu_time;
	}
	sched_print(print, n, time);
}

void sjf_func(process* pro, int n) {
	int i, j,k, type=0, time, rem_pro, print[MAX][SIZE],index, min_pro = 0, min, rem[MAX];
	
	//save remain time
	for(i=0; i<n; i++) {
		rem[i] = pro[i].cpu_time;
		pro[i].wait_time = 0;
	}
	
	time = pro[0].arr_time;
	rem_pro = n;
	
	//sjf
	for (i=0; i<n; i++) {
		min = 1000;
		type = 0;
		for (j=0; j<n; j++) {
			if (rem[j] != 0 && pro[j].cpu_time < min && time >= pro[j].arr_time) {
				min_pro = j;
				min = pro[j].cpu_time;
				type = 1;
			}
		}
		if (type == 1) {
			rem[min_pro] = 0;
			pro[min_pro].wait_time = time - pro[min_pro].arr_time;
			//print insert
			for (j=0; j<n; j++) {
				index = time;
				for (k=0; k< pro[min_pro].cpu_time; k++) {
					if (j==i) {
						print[j][index] = 1;
						index++;
					}
					else {
						print[j][index] = 0;
						index++;
					}
				}
			}
			time += pro[min_pro].cpu_time;
			rem_pro--;
		}
		else {
			time++;
		}
	}
	sched_print(print,n, time);
}

void rr_func(process* pro, int n, int q) {
	int i, j, k;
	int rem[MAX], pro_num=0, be_time=0, af_time=0;
	int print[MAX][SIZE], index;

	Ready r;
	InitQueue(&r);	//Initialize Queue
	
	//save remain time
	for (i=0; i<n; i++) {
		rem[i] = pro[i].cpu_time;
		pro[i].wait_time = 0;
		pro[i].total_time = 0;
	}
	Enqueue(&r, 0);		//first job
	
	while (1) {
		if (isEmpty(&r)) {	//if all job done
			break;
		}
		else {
			pro_num = Dequeue(&r);
		}
		
		if (rem[pro_num] > q) {		//remain time is left
			for (j=0; j<n; j++) {
				index = af_time;
				for (k=0; k<q; k++) {
					if (j == pro_num) {
						print[j][index] = 1;
						index++;
					}
					else {
						print[j][index] = 0;
						index++;
					}
				}
			}
			rem[pro_num] -= q;
			be_time = af_time;	//save before time
			af_time += q;		//add cpu time
			
			for (i=0; i<n; i++) {
				if (pro[i].arr_time > be_time && pro[i].arr_time <= af_time) {
					Enqueue(&r, i);
				}
			}
			Enqueue(&r, pro_num);
		}
		else if (rem[pro_num] > 0) {	//this job is done
			for (j=0; j<n; j++) {
				index = af_time;
				for (k=0; k<rem[pro_num]; k++) {
					if (j == pro_num) {
						print[j][index] = 1;
						index++;
					}
					else {
						print[j][index] = 0;
						index++;
					}
				}
			}
			be_time = af_time;
			af_time += rem[pro_num];
			pro[pro_num].total_time = af_time - pro[pro_num].arr_time;
			rem[pro_num] = 0;

			for (i=0; i<n; i++) {
				if (pro[i].arr_time > be_time && pro[i].arr_time <= af_time) {
					Enqueue(&r, i);
				}
			}
		}
	}
	for (i=0; i<n; i++) {	//find remain time
		pro[i].wait_time = pro[i].total_time - pro[i].cpu_time;
	}
	sched_print(print, n, af_time);	//print function
}



void mlfq_func(process* pro, int n, int q) {
	int i, j, k;
	int rem[MAX], pro_num = 0, be_time = 0, af_time = 0;
	int print[MAX][SIZE], index;
	Ready Q0, Q1, Q2;
	InitQueue(&Q0); //init Queue 3
	InitQueue(&Q1);
	InitQueue(&Q2);

	for (i = 0; i < n; i++) {
		rem[i] = pro[i].cpu_time;
		pro[i].wait_time = 0;		//init wait time, total time, pri
		pro[i].total_time = 0;
		pro[i].pri = 1;	
	}
	Enqueue(&Q0, 0);	//first job


	while (1) {

		if (isEmpty(&Q0) == false) {
			pro_num = Dequeue(&Q0);
		}
		else if (isEmpty(&Q1) == false) {
			pro_num = Dequeue(&Q1);
		}
		else if (isEmpty(&Q2) == false)
			pro_num = Dequeue(&Q2);
		else
			break;

		if (rem[pro_num] > q) {		//job is still remain
			for (j = 0; j < n; j++) {
				index = af_time;
				for (k = 0; k < q; k++) {
					if (j == pro_num) { 
						print[j][index] = 1;
						index++;
					}
					else {
						print[j][index] = 0;
						index++;
					}
				}
			}
			rem[pro_num] -= q;
			be_time = af_time;
			af_time += q;	//current time

			for (i = 0; i < n; i++) {
				if (pro[i].arr_time > be_time && pro[i].arr_time <= af_time) {
					//find new process 
					Enqueue(&Q0, i);
				}
			}
			// old process add 
			if (pro[pro_num].pri == 1) {
				pro[pro_num].pri++;
				Enqueue(&Q1, pro_num);
			}
			else if (pro[pro_num].pri == 2) {
				pro[pro_num].pri++;
				Enqueue(&Q2, pro_num);
			}
			else
				Enqueue(&Q2, pro_num);
		}
		else if (rem[pro_num] > 0) {	//job is done
			for (j = 0; j < n; j++) {
				index = af_time;
				for (k = 0; k < rem[pro_num]; k++) {
					if (j == pro_num) { 
						print[j][index] = 1;
						index++;
					}
					else {
						print[j][index] = 0;
						index++;
					}
				}
			}
			be_time = af_time; 
			af_time += rem[pro_num]; //currnet time
			pro[pro_num].total_time = af_time - pro[pro_num].arr_time;
			rem[pro_num] = 0;

			for (i = 0; i < n; i++) {
				if (pro[i].arr_time > be_time && pro[i].arr_time <= af_time) {
					Enqueue(&Q0, i);
				}
			}
		}

	}
	for (i = 0; i < n; i++) {	//find wait time
		pro[i].wait_time = pro[i].total_time - pro[i].cpu_time;
	}
	sched_print(print, n, af_time);	//print scheduling
}
