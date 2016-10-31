#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE	1
#define FALSE	0

struct queueNode {
	int key;
	struct queueNode *next;
};

struct queueHeader {
	struct queueNode *head;
	struct queueNode *tail;
};

void *hwthread(void *arg);
void enqueue(struct queueHeader *qh, int key);
int* dequeue(struct queueHeader *qh);
int isEmpty(struct queueHeader *qh);

struct queueHeader Qheader = {NULL,};
pthread_mutex_t lock;

int main(int argc, char* argv[]) {
	struct timeval te;
	long long milliseconds;
	pthread_t p1, p2, p3, p4;
	
	gettimeofday(&te, NULL);
	//milliseconds = ((te.tv_sec) * 100000) + te.tv_usec;
	milliseconds = clock();
	
	printf("%lld start\n", milliseconds);

	pthread_mutex_init(&lock, NULL);

	pthread_create(&p1, NULL, hwthread, NULL);
	pthread_create(&p2, NULL, hwthread, NULL);
	pthread_create(&p3, NULL, hwthread, NULL);
	pthread_create(&p4, NULL, hwthread, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);

	if(!isEmpty(&Qheader))
		printf(" Q isn't empty\n");
	
	gettimeofday(&te, NULL);
	//milliseconds = ((te.tv_sec) * 100000) + te.tv_usec;
	milliseconds = clock();
	
	printf("%lld end\n", milliseconds);

	return 0;
}

void *hwthread(void *arg) {
	int i = 0;
	int *key = NULL;
	
	for (i = 0; i<1000000;i++){
		enqueue(&Qheader, i);
		key = dequeue(&Qheader);
		if (key) 
			free(key);
		else
			printf("queue empty");
	}

}

void enqueue(struct queueHeader *qh, int key) {
	struct queueNode *newNode = (struct queueNode *)malloc(sizeof(struct queueNode));
	pthread_mutex_lock(&lock);

	newNode->key = key;
	newNode->next = NULL;
	
	if (!qh->head)
		qh->head = newNode;
	else 
		qh->tail->next = newNode;
	
	qh->tail = newNode;
	pthread_mutex_unlock(&lock);
}

int* dequeue(struct queueHeader *qh) {
	pthread_mutex_lock(&lock);
	
	if (qh->head) {
		int *returnval = (int *)malloc(sizeof(int));
		struct queueNode *delNode = qh->head;

		*returnval = qh->head->key;
		if (qh->head == qh->tail)
			qh->tail = NULL;
		qh->head = qh->head->next;
		delNode->next = NULL;

		free(delNode);
		pthread_mutex_unlock(&lock);

		return returnval;
	}
	else {
		pthread_mutex_unlock(&lock);
		return NULL;
	}
}


int isEmpty(struct queueHeader *qh) {
	if (!qh->head && !qh->tail) 
		return TRUE;
	else
		return FALSE;
}
