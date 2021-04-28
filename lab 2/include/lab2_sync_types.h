/*   DKU Operating System Lab
*           Lab2 (Vehicle Production Problem)
*           Student id : 32195044
*           Student name : Hwang Yejin
*
*   lab1_sync_types.h :
*       - lab2 header file.
*       - must contains Vehicle Production Problem's declations.
*
*/

#ifndef _LAB2_HEADER_H
#define _LAB2_HEADER_H

#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#define MAX_SIZE 100
#define T_count 5
#define MAX 10

/*
 * You need to Declare functions in  here
 */

typedef struct Node {
	int car_num;
	struct Node *next;
}Node;

typedef struct car_queue {
	int balance;
	Node *front, *rear;
}CQ;

typedef struct Pro_info {
	int num;
}PI;

typedef struct Con_info {
	int num;
}CI;

void put(int value);
int get();
void *producer(void *arg);
void *consumer(void *arg);




#endif /* LAB2_HEADER_H*/


