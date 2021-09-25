#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>


struct T{
	int num;
	int age;
};

//注意此处的回调函数，参数和返回值都是void*类型
void* callback(void* args)
{
	struct T* t = (struct T*)args;

	for(int i=0;i<5;i++)
	{
		printf("子线程 thread: %d\n",i);
	}
	printf("子线程 thread ID: %ld\n",pthread_self());

	t->num=123456;
	t->age=18;
	pthread_exit(t);

	return NULL;
}

int main()
{
	
	struct T t;

	pthread_t tid;
	pthread_create(&tid,NULL,callback,&t);//传参
	
	printf("主线程 thread ID: %ld\n",pthread_self());

	pthread_detach(tid);//分离子线程
	pthread_exit(NULL);//线程退出不影响其他线程

	/*
	pthread_join(tid,NULL);
	printf("num:%d age:%d\n",t.num,t.age);
	*/
	return 0;
}
