/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32195044
*	    Student name : Hwang Yejin
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H
#define SIZE 100
#define MAX 20

typedef struct _process_info {
	int id;
	int cpu_time;
	int arr_time;
	int wait_time;
	int total_time;
	int pri;
} process;

typedef struct Node {
	int data;
	struct Node* next;
} Node;

typedef struct Ready {
	Node* front;
	Node* rear;
	int count;
} Ready;

//Queue's function
void InitQueue(Ready* r);
int isEmpty(Ready* r);
void Enqueue(Ready* r, int num);
int Dequeue(Ready* r);

//Scheduling result print function
void sched_print(int print[][SIZE], int n, int time);

//Scheduling function
void fifo_func(process* pro, int n);
void sjf_func(process* pro, int n);
void rr_func(process* pro, int n, int q);
void mlfq_func(process* pro, int n, int q);


#endif /* LAB1_HEADER_H*/



