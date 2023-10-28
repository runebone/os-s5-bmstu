#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define N 2
#define SLEEP_FOR 2

int main()
{
    pid_t child_pid[N];
    pid_t child;
    int status;

    /* printf("parent pid: %d, group: %d\n", getpid(), getpgrp()); */

    for (int i = 0; i < N; i++) 
    {
        child_pid[i] = fork();

        if (child_pid[i] == -1) 
        {
            perror("can't fork\n");
            exit(1);
        }

        if (child_pid[i] == 0) 
        {
            printf("B child %d pid: %d, group %d, ppid: %d\n",
                    i, getpid(), getpgrp(), getppid());
            /* sleep(SLEEP_FOR); */
            if (i == 0) while (1);
            printf("E child %d pid: %d, group %d, ppid: %d\n",
                    i, getpid(), getpgrp(), getppid());

            return 0;
        }
        else
        {
            printf("parent pid: %d, child %d %d, group %d\n",
                    getpid(), i, child_pid[i], getpgrp());
        }
    }

    for (int i = 0; i < N; i++) 
    {
        child = wait(&status);
        // printf("parent pid: %d\nchildren %d\n", getpid(), child_pid[i]);
        printf("parent: child %d pid: %d, parent %d, status: %d, group %d\n",
                i, child, getpid(), status, getpgrp());

        if (WIFEXITED(status)) 
        {
            printf("parent: child %d exited with code %d, child pid: %d\n",
                    i, WEXITSTATUS(status), child);
        }
        else if (WIFSIGNALED(status)) 
        {
            printf("parent: child %d terminated with un-intercepted signal number %d, child pid: %d\n",
                    i, WTERMSIG(status), child);
        }
        else if (WIFSTOPPED(status)) 
        {
            printf("parent: child %d stopped with signal number %d, child pid: %d\n",
                    i, WSTOPSIG(status), child);
        }
    }

    return 0;
}
