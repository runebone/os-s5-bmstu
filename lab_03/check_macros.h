#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void check_macros(pid_t child)
{
    int status;

    pid_t w = waitpid(child, &status, 0);
    /* printf("parent: child pid: %d, parent %d, status: %d, group %d\n", */
    /*         child, getpid(), status, getpgrp()); */

    if (WIFEXITED(status)) 
    {
        printf("%d  exited with code %d\n", w,//,  pid: %d\n",
                WEXITSTATUS(status));//, );
    }
    else if (WIFSIGNALED(status)) 
    {
        printf("%d  terminated with un-intercepted signal number %d\n", w,//,  pid: %d\n",
                WTERMSIG(status));//, );
    }
    else if (WIFSTOPPED(status)) 
    {
        printf("%d  stopped with signal number %d\n", w,//,  pid: %d\n",
                WSTOPSIG(status));//, );
    }
}
