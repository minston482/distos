#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE	1
#define FALSE	0
#define START_KEY	-897
#define INIT_KEY	-132

struct queueNode {
	int key;
	struct queueNode *next;
};

struct queueHeader {
	struct queueNode *head;
	struct queueNode *tail;
	pthread_mutex_t h_lock;
	pthread_mutex_t t_lock;
};

void *hwthread(void *arg);
void enqueue(struct queueHeader *qh, int key);
int* dequeue(struct queueHeader *qh);
int isEmpty(struct queueHeader *qh);

struct queueHeader Qheader = {NULL,};

int main(int argc, char* argv[]) {
	struct timeval te;
	long long milliseconds;
	int thread_size = atoi(argv[1]), i = 0;
	pthread_t *p = (pthread_t *) malloc (sizeof(pthread_t) * thread_size);
	struct queueNode *startNode = (struct queueNode *)malloc(sizeof(struct queueNode));
	
	gettimeofday(&te, NULL);
	milliseconds = clock();
	printf("%lld start\n", milliseconds);

	startNode->key = START_KEY;
	startNode->next = NULL;
	Qheader.head = startNode;
	Qheader.tail = startNode;

	for(i = 0; i < thread_size; i++) 
		pthread_create(&p[i], NULL, hwthread, NULL);
	
	for(i = 0; i < thread_size; i++) 
		pthread_join(p[i], NULL);

	if(!isEmpty(&Qheader))
		printf(" Q isn't empty\n");
	
	gettimeofday(&te, NULL);
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
			printf("queue empty\n");
	}
}

void enqueue(struct queueHeader *qh, int key) {
	struct queueNode *newNode = (struct queueNode *)malloc(sizeof(struct queueNode));
	
	newNode->key = key;
	newNode->next = NULL;

	pthread_mutex_lock(&qh->t_lock);
	qh->tail->next = newNode;
	qh->tail = newNode;
	pthread_mutex_unlock(&qh->t_lock);
	
}

int* dequeue(struct queueHeader *qh) {
	struct queueNode *delNode = NULL, *preheadNode = NULL;
	
	pthread_mutex_lock(&qh->h_lock);
	preheadNode = qh->head;
	if (qh->head->next) {
		int *retval = (int *)malloc(sizeof(int));
		delNode = qh->head->next;
		*retval = delNode->key;
		qh->head->next = delNode->next;
		qh->head = delNode;

		preheadNode->key = INIT_KEY;
		preheadNode->next = NULL;

		free(preheadNode);

		pthread_mutex_unlock(&qh->h_lock);
		return retval;
	} else {
		pthread_mutex_unlock(&qh->h_lock);
		return NULL;
	}
}


int isEmpty(struct queueHeader *qh) {
	if (!qh->head->next && !qh->tail->next) 
		return TRUE;
	else
		return FALSE;
}
