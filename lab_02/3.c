#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define N 2

/* char *exec_params[2] = {"./p1", "./cg"}; */
char *exec_params[2] = {"./p1", "./p2"};

void check_macros(pid_t child)
{
    int status;

    for (int i = 0; i < N; i++) 
    {
        /* child = wait(&status); */

        waitpid(child, &status, 0);

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
}

int main()
{
    pid_t child_pid[N];
    int rc, status;

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
            printf("child %d pid: %d, group %d, ppid: %d\n",
                    i, getpid(), getpgrp(), getppid());
            printf("child %d (pid = %d) executed %s\n",
                    i, getpid(), exec_params[i]);

            rc = execl(exec_params[i], "");

            if (rc == -1)
            {
                perror("cant exec\n");
                exit(1);
            }

            /* return 0; */
            exit(0);
        }
        else
        {
            printf("parent pid: %d, child %d %d, group %d\n",
                    getpid(), i, child_pid[i], getpgrp());
        }
    }

    /* waitpid(child_pid[0], &status, 0); */
    /* waitpid(child_pid[1], &status, 0); */

    check_macros(child_pid[0]);
    check_macros(child_pid[1]);

    printf("finished: parent pid: %d, group: %d\n", getpid(), getpgrp());
   
    return 0;
}
