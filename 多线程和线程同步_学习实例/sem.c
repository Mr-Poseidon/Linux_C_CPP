#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

pthread_mutex_t mtx;
sem_t semp;
sem_t semc;

struct Node{
	int num;
	struct Node* next;
};
struct Node* head=NULL;



void* producer(void* arg)
{
	while(1)
	{
		sem_wait(&semp);//生产者信号量--
		pthread_mutex_lock(&mtx);//给互斥锁加锁
		struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
		newnode->num = rand()%1000;
		printf("生产者 ID: %lu Number:%d \n",pthread_self(),newnode->num);
		newnode->next = head;
		head = newnode;
		pthread_mutex_unlock(&mtx);//解开互斥锁
		sem_post(&semc);//消费者信号量++
		sleep(rand()%3);
	}

	return NULL;
}

void* consumer(void* arg)
{
	while(1)
	{
		sem_wait(&semc);//消费者信号量--
		pthread_mutex_lock(&mtx);
		struct Node* node=head;
		printf("消费者 ID: %lu Number:%d \n",pthread_self(),node->num);
		head = head->next;
		free(node);
		pthread_mutex_unlock(&mtx);
		sem_post(&semp);//生产者信号量++
		sleep(rand()%3);
	}
	return NULL;
}

int main()
{
	sem_init(&semp,0,5);//生产者信号量
	sem_init(&semc,0,0);//消费者信号量

	pthread_mutex_init(&mtx,NULL);

	pthread_t p1[5],p2[5];
	for(int i=0;i<5;i++)
	{
		pthread_create(&p1[i],NULL,producer,NULL);
	}
	for(int i=0;i<5;i++)
	{
		pthread_create(&p2[i],NULL,consumer,NULL);
	}




	for(int i=0;i<5;i++)
	{		
		pthread_join(p1[i],NULL);
		pthread_join(p2[i],NULL);
	}

	pthread_mutex_destroy(&mtx);
	sem_destroy(&semp);
	sem_destroy(&semc);

	return 0;
}
