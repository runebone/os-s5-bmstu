#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "bakery.h"
#include <time.h>

#include <sys/syscall.h>

pid_t gettid()
{
    return syscall(SYS_gettid);
}

static int cur = 0;
static char ch = 'a';
static int client_is_getting_ticket[128] = {0};
static int numbers[128] = {0};
static int pids[128] = {0};

int get_max_ticket_number()
{
	int max_res = 0;
	for (int i = 0; i < 128; i++)
		if (numbers[i] > max_res)
			max_res = numbers[i];
	return max_res;
}

void *get_ticket(void *arg)
{
	time_t mytime = time(NULL);
	static struct REQUEST  result;
	struct REQUEST *argp = arg;

	argp->index = cur;
    result.index = cur;
	cur++;

    result.pid = argp->pid;
	pids[argp->index] = argp->pid;

	client_is_getting_ticket[argp->index] = 1;
	result.number = get_max_ticket_number() + 1;
	numbers[argp->index] = result.number;
	client_is_getting_ticket[argp->index] = 0;

	struct tm *now = localtime(&mytime);
	struct timeval tv;
	gettimeofday(&tv, NULL);

    printf("Thread %d gave ticket %d to client with pid %d at %02d:%02d:%02d:%03ld\n",
            gettid(), result.number, result.pid,
            now->tm_hour, now->tm_min, now->tm_sec, tv.tv_usec / 1000);

	return &result;
}
struct REQUEST *get_number_1_svc(struct REQUEST *p_arg, struct svc_req *rqstp)
{
	static struct REQUEST  result;
	void *tmp;
	int c;
	pthread_t thread;
	pthread_attr_t attr;

	c = pthread_attr_init(&attr);
	if (c != 0)
	{
		perror("pthread_attr_init");
		exit(1);
	}
	c = pthread_create(&thread, &attr, get_ticket, p_arg);
	if (c != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	c = pthread_attr_destroy(&attr);
	if (c != 0)
	{
		perror("pthread_attr_destroy");
		exit(1);
	}
	c = pthread_join(thread, &tmp);
	if (c != 0)
	{
		perror("pthread_join");
		exit(1);
	}
	result = *(struct REQUEST *)tmp;
	return &result;
}
void *bakery_service(void *arg)
{
	time_t mytime = time(NULL);

	static int  result;
	struct REQUEST *argp = arg;
	for (int i = 0; i < 128; i++)
	{
		while (client_is_getting_ticket[i]) {}
		while (numbers[i] != 0 && (numbers[i] < numbers[argp->index] || 
								   numbers[i] == numbers[argp->index] && 
								   pids[i] < pids[argp->index])) {}
	}
	result = ch++;
	numbers[argp->index] = 0;

	struct tm *now = localtime(&mytime);
	struct timeval tv;
	gettimeofday(&tv, NULL);

    printf("Thread %d served client with pid %d at %02d:%02d:%02d:%03ld with '%c'\n",
            gettid(), pids[argp->index], now->tm_hour, now->tm_min, now->tm_sec, tv.tv_usec / 1000, result);

	pthread_exit(&result);
}
int *bakery_service_1_svc(struct REQUEST *p_arg, struct svc_req *rqstp)
{
	static int  result;
	void *tmp;
	int c;
	pthread_t thread;
	pthread_attr_t attr;

	c = pthread_attr_init(&attr);
	if (c != 0)
	{
		perror("pthread_attr_init");
		exit(1);
	}
	c = pthread_create(&thread, &attr, bakery_service, p_arg);
	if (c != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	c = pthread_attr_destroy(&attr);
	if (c != 0)
	{
		perror("pthread_attr_destroy");
		exit(1);
	}
	c = pthread_join(thread, &tmp);
	if (c != 0)
	{
		perror("pthread_join");
		exit(1);
	}

	result = *(int *)tmp;
	return &result;
}
