/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "bakery.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
bakery_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		client_data get_ticket_1_arg;
		client_data get_service_1_arg;
	} argument;
	union {
		service_data get_service_1_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case get_ticket:
		_xdr_argument = (xdrproc_t) xdr_client_data;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (bool_t (*) (char *, void *,  struct svc_req *))get_ticket_1_svc;
		break;

	case get_service:
		_xdr_argument = (xdrproc_t) xdr_client_data;
		_xdr_result = (xdrproc_t) xdr_service_data;
		local = (bool_t (*) (char *, void *,  struct svc_req *))get_service_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char *)&result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	if (!bakery_prog_1_freeresult (transp, _xdr_result, (caddr_t) &result))
		fprintf (stderr, "%s", "unable to free results");

	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (BAKERY_PROG, BAKERY_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, BAKERY_PROG, BAKERY_VERS, bakery_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (BAKERY_PROG, BAKERY_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, BAKERY_PROG, BAKERY_VERS, bakery_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (BAKERY_PROG, BAKERY_VERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
