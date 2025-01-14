#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t count_lock;//互斥锁
pthread_cond_t count_nonzero;//条件变量
unsigned count = 0;
void * decrement_count(void *arg)
{
	pthread_mutex_lock (&count_lock);
	printf("decrement_count get count_lock\n");
	while (count==0) 
	{
		printf("decrement_count count == 0 \n");
		printf("decrement_count before cond_wait\n");
		pthread_cond_wait( &count_nonzero, &count_lock);//线程阻塞前会解锁，唤醒前会上锁
		printf("decrement_count after cond_wait \n");
	}
	count = count -1;
	pthread_mutex_unlock (&count_lock);
}

void * increment_count(void *arg)
{
	pthread_mutex_lock(&count_lock);
	printf("increment_count get count_lock\n");
	if (count == 0) 
	{
		printf("increment_count before cond_signal\n");
		pthread_cond_signal(&count_nonzero);//唤醒正在等待在条件变量count_nonzero上的线程
		printf("increment_count after cond_signal\n");
	}
	count=count+1;
	pthread_mutex_unlock(&count_lock);
}

int main(void)
{
	pthread_t tid1,tid2;

	pthread_mutex_init(&count_lock,NULL);
	pthread_cond_init(&count_nonzero,NULL);

	pthread_create(&tid1,NULL,decrement_count,NULL);
	sleep(2);
	pthread_create(&tid2,NULL,increment_count,NULL);

	sleep(10);
	pthread_exit(0);
}
