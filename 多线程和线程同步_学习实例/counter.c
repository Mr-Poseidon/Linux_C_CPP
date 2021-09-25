#include<stdio.h>//sleep()以秒为单位挂起
#include<stdlib.h>
#include<unistd.h>//usleep()以微秒为单位挂起
#include<pthread.h>


int number=0;
pthread_mutex_t mtx;



void* funA(void* arg)
{
	for(int i=0;i<50;i++)
	{
		pthread_mutex_lock(&mtx);
		int num=number;
		num++;
		number=num;//通过sleep模仿线程的复杂操作，复现线程同时访问共享内存的情况
		printf("线程A ID:%lu number:%d\n",pthread_self(),number);
		pthread_mutex_unlock(&mtx);
		usleep(5);
	}
	return NULL;
}



void* funB(void* arg)
{
	for(int i=0;i<50;i++)
	{
		pthread_mutex_lock(&mtx);
		int num=number;
		num++;
		usleep(3);//通过sleep模仿线程的复杂操作，复现线程同时访问共享内存的情况
		number=num;
		printf("线程B ID:%lu number:%d\n",pthread_self(),number);
		pthread_mutex_unlock(&mtx);
	}
	return NULL;
}

int main()
{
	pthread_mutex_init(&mtx,NULL);

	pthread_t p1,p2;
	pthread_create(&p1,NULL,funA,NULL);
	pthread_create(&p2,NULL,funB,NULL);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);

	pthread_mutex_destroy(&mtx);

	return 0;
}
