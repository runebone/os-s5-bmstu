/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "bakery.h"

#include <pthread.h>
#include <unistd.h>
#include <memory.h>

static int next_ticket = 0;

static char bun_names[][255] = {
    "Солёная булка",
    "Сладкая булка",
    "Горькая булка",
    "Кислая булка",
    "Острая булка",
    "Вяжущая булка"
};

bool_t get_ticket_1_svc(client_data *argp, client_data *result, struct svc_req *rqstp)
{
	bool_t retval;

    printf("Giving ticket %d to client (pid: %d)\n", next_ticket, argp->pid);

    // @Note: Mutex maybe?
    argp->ticket_number = next_ticket++;

#if 1
    printf("DBG: client_data addr: %p, pid: %d\n", argp, argp->pid);
#endif

    result = argp;

	return retval;
}

struct thread_data
{
    client_data *argp;
    service_data *result;
};
typedef struct thread_data thread_data;

void get_service_1_svc_logic(void *arg)
{
    thread_data *targ = (thread_data*)arg;
    client_data *argp = targ->argp;
    service_data *result = targ->result;

    memcpy(result->bun_name, bun_names[argp->ticket_number % 6], 255);
    result->served_by_pid = getpid();
}

bool_t get_service_1_svc(client_data *argp, service_data *result, struct svc_req *rqstp)
{
	bool_t retval;

    thread_data tdata = { argp, result };
    get_service_1_svc_logic(&tdata);

	return retval;
}

int bakery_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
