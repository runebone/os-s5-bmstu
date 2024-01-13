#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include "bakery.h"

void bakery_prog_1(char *host)
{
	CLIENT *clnt;
	struct REQUEST  *result_1;
	struct REQUEST  request;

	request.pid = getpid();

	int  *result_2;

#ifndef	DEBUG
	clnt = clnt_create (host, BAKERY_PROG, BAKERY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	srand(time(NULL));
	usleep(rand() % 1000 + 5000000);

	result_1 = get_number_1(&request, clnt);
	if (result_1 == (struct REQUEST *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	request.index = result_1->index;
	request.number = result_1->number;
	printf("Client (pid: %d) got ticket %d\n", request.pid, request.number);

	usleep(rand() % 1000 + 5000000);
	result_2 = bakery_service_1(&request, clnt);
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("Client (pid: %d) with ticket %d got served with: %c\n", request.pid, request.number, *result_2);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

int main(int argc, char *argv[])
{
	char *host;
	if (argc < 2) {
        host = "localhost";
	} else if (argc == 2) {
        host = argv[1];
    } else {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
    }
    printf("pid: %d\n", getpid());
	bakery_prog_1(host);
exit (0);
}
