#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_cond_t cond;
pthread_mutex_t mtx;

struct Node{
	int num;
	struct Node* next;
};
struct Node* head=NULL;



void* producer(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mtx);//给互斥锁加锁
		struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
		newnode->num = rand()%1000;
		printf("生产者 ID: %lu Number:%d \n",pthread_self(),newnode->num);
		newnode->next = head;
		head = newnode;
		pthread_mutex_unlock(&mtx);//解开互斥锁
		pthread_cond_broadcast(&cond);//唤醒条件变量阻塞的所有消费者线程
		sleep(rand()%3);
	}

	return NULL;
}

void* consumer(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mtx);
		while(head==NULL)//此处使用while可以保证唤醒后直接进入，必须要再次判断，保证线程安全
		{
			pthread_cond_wait(&cond,&mtx);//阻塞线程,此处阻塞线程后会打开互斥锁
		}
		struct Node* node=head;
		printf("消费者 ID: %lu Number:%d \n",pthread_self(),node->num);
		head = head->next;
		free(node);
		pthread_mutex_unlock(&mtx);
		sleep(rand()%3);
	}
	return NULL;
}

int main()
{
	pthread_cond_init(&cond,NULL);
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

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mtx);
	return 0;
}
