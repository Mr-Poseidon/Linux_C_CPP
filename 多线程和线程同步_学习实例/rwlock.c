#include<stdio.h>//sleep()以秒为单位挂起
#include<stdlib.h>
#include<unistd.h>//usleep()以微秒为单位挂起
#include<pthread.h>


int number=0;

pthread_rwlock_t rwlock;

void* read_fun(void* arg)
{
	for(int i=0;i<50;i++)
	{
		pthread_rwlock_rdlock(&rwlock);
		printf("线程A ID:%lu number:%d\n",pthread_self(),number);
		pthread_rwlock_unlock(&rwlock);
		usleep(rand()%5);
	}
	return NULL;
}



void* write_fun(void* arg)
{
	for(int i=0;i<50;i++)
	{
		pthread_rwlock_wrlock(&rwlock);
		int num=number;
		num++;
		usleep(3);//通过sleep模仿线程的复杂操作，复现线程同时访问共享内存的情况
		number=num;
		printf("写线程 ID:%lu number:%d\n",pthread_self(),number);
		pthread_rwlock_unlock(&rwlock);
	}
	return NULL;
}

int main()
{
	pthread_rwlock_init(&rwlock,NULL);

	pthread_t p1[5],p2[3];
	for(int i=0;i<5;i++)
		pthread_create(&p1[i],NULL,read_fun,NULL);
	for(int i=0;i<3;i++)
		pthread_create(&p2[i],NULL,write_fun,NULL);

	for(int i=0;i<5;i++)
		pthread_join(p1[i],NULL);
	for(int i=0;i<3;i++)
		pthread_join(p2[i],NULL);

	pthread_rwlock_destroy(&rwlock);

	return 0;
}
