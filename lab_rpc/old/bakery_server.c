/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "bakery.h"

#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/syscall.h>

pid_t gettid()
{
    return syscall(SYS_gettid);
}

static int next_ticket = 0;

static char buns[][255] = {
    "Сладкая булка",
    "Солёная булка",
    "Горькая булка",
    "Кислая булка",
    "Острая булка",
    "Вяжущая булка"
};

static int is_client_getting_ticket[50] = { 0 };
static int client_ticket_number[50] = { 0 };

struct thread_data_ticket
{
    client_data *argp;
    client_data *result;
    bool_t retval;
};
typedef struct thread_data_ticket thread_data_ticket;

struct thread_data_bakery
{
    client_data *argp;
    service_data *result;
    bool_t retval;
};
typedef struct thread_data_bakery thread_data_bakery;

void *get_ticket_thread_func(void *arg)
{
    thread_data_ticket *targ = (thread_data_ticket*)arg;
    client_data *argp = targ->argp;
    client_data *result = targ->result;
    bool_t *retval = &targ->retval;

	time_t mytime = time(NULL);

    if (next_ticket == 50)
    {
        printf("Tickets are sold out!\n");
        pthread_exit((void*)1);
    }

    for (int i = 0; i < 50; i++)
    {
        while(is_client_getting_ticket[i]) {}
    }

    is_client_getting_ticket[next_ticket] = 1;
    result->pid = argp->pid;
    result->ticket_number = next_ticket;
    client_ticket_number[next_ticket] = next_ticket + 1;
    if (client_ticket_number[next_ticket] != 0)
    {
        // In case something went wrong and we are attempting the same request,
        // don't increment ticket number
    }
    else
    {
        ++next_ticket;
    }
    is_client_getting_ticket[next_ticket - 1] = 0;

	struct tm *now = localtime(&mytime);
	struct timeval tv;
	gettimeofday(&tv, NULL);

    printf("Thread %d gave ticket %d to client with pid %d at %02d:%02d:%02d:%03ld\n",
            gettid(), result->ticket_number + 1, result->pid,
            now->tm_hour, now->tm_min, now->tm_sec, tv.tv_usec / 1000);

    return NULL;
}

bool_t get_ticket_1_svc(client_data *argp, client_data *result, struct svc_req *rqstp)
{
	bool_t retval;

    thread_data_ticket tdata;
    tdata.argp = argp;
    tdata.result = result;

    pthread_t thread;
    pthread_attr_t attr;

    int c = pthread_attr_init(&attr);
    if (c != 0)
    {
        perror("pthread_attr_init");
        exit(1);
    }
    c = pthread_create(&thread, &attr, get_ticket_thread_func, &tdata);
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
    c = pthread_join(thread, NULL);
    if (c != 0)
    {
        perror("pthread_join");
        exit(1);
    }
    printf("get_ticket_1_svc done\n");

	return retval;
}

void *get_service_thread_func(void *arg)
{
    thread_data_bakery *targ = (thread_data_bakery*)arg;
    client_data *argp = targ->argp;
    service_data *result = targ->result;
    bool_t *retval = &targ->retval;

	time_t mytime = time(NULL);

    for (int i = 0; i < 50; i++)
    {
        while(is_client_getting_ticket[i]) {}
    }

	struct tm *now = localtime(&mytime);
	struct timeval tv;
	gettimeofday(&tv, NULL);

    printf("Thread tid %d serves client with ticket %d\n", gettid(), argp->ticket_number + 1);

    memcpy(result->bun_name, buns[argp->ticket_number % 6], 255);
    result->served_by_pid = getpid();
    result->served_by_tid = gettid();
    sleep(5);

    return NULL;
}

bool_t get_service_1_svc(client_data *argp, service_data *result, struct svc_req *rqstp)
{
	bool_t retval;

    thread_data_bakery tdata;
    tdata.argp = argp;
    tdata.result = result;

    pthread_t thread;
    pthread_attr_t attr;

    int c = pthread_attr_init(&attr);
    if (c != 0)
    {
        perror("pthread_attr_init");
        exit(1);
    }
    c = pthread_create(&thread, &attr, get_service_thread_func, &tdata);
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
    c = pthread_join(thread, (void*)&retval);
    if (c != 0)
    {
        perror("pthread_join");
        exit(1);
    }

	return retval;
}

int bakery_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free(xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
