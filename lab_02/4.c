#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "colors.h"

#define N 2

void check_macros(pid_t child)
{
    int status;

    for (int i = 0; i < N; i++) 
    {
        waitpid(child, &status, 0);
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

int main(void) 
{
    pid_t child_pid[N];
    int fd[N];
    int status;
    int child;
    char *msg1 = "aaa\n";
    char *msg2 = "yyyyy\n";
    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    if (pipe(fd) == -1) 
    {
        perror("cant pipe\n");
        exit(1);
    }

    /* for (int i = 0; i < N; i++) */ 
    {
        child_pid[0] = fork();

        if (child_pid[0] == -1) 
        {
            perror("cant fork\n");
            exit(1);
        } 
        if (child_pid[0] == 0) 
        {
            close(fd[0]);
            write(fd[1], *msg1, sizeof(*msg1));
            exit(0);
        }
        else
        {
            printf("parent pid: %d, child %d %d, group %d\n", getpid(), 0, child_pid[0], getpgrp());
            close(fd[1]);
            read(fd[0], *msg1, sizeof(*msg1));
            printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg1);  
        }
    }

    {
        child_pid[1] = fork();

        if (child_pid[1] == -1) 
        {
            perror("cant fork\n");
            exit(1);
        } 
        if (child_pid[1] == 0) 
        {
            close(fd[0]);
            write(fd[1], *msg2, sizeof(*msg2));
            exit(0);
        }
        else
        {
            printf("parent pid: %d, child %d %d, group %d\n", getpid(), 1, child_pid[1], getpgrp());
            close(fd[1]);
            read(fd[0], *msg2, sizeof(*msg2));
            printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg2);  
            msg2 = "";
            read(fd[0], *msg2, sizeof(*msg2));
            printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg2);  
        }
    }

    for (int i = 0; 1 && i < N; i++) 
    {
        /* child = wait(&status); */

        /* waitpid(child_pid[i], &status, 0); */

        check_macros(child_pid[i]);

        /* close(fd[1]); */
        /* read(fd[0], *msg, sizeof(*msg)); */
        /* if (i == 0) { */
        /*     read(fd[0], *msg1, sizeof(*msg1)); */
        /*     printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg1); */  
        /* } else if (i == 1) { */
        /*     read(fd[0], *msg2, sizeof(*msg2)); */
        /*     printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg2); */  
        /* } */
        /* printf("parent (pid: %d) recieved msg: %s\n", getpid(), *msg); */  

        /* printf("child %d pid: %d, parent %d, status: %d, group %d\n", i, child, getpid(), status, getpgrp()); */

        /* if (WIFEXITED(status)) */ 
        /* { */
        /*     printf("child %d exited with code %d, child pid: %d\n", i, WEXITSTATUS(status), child); */
        /* } */
        /* else if (WIFSIGNALED(status)) */ 
        /* { */
        /*     printf("child %d terminated with un-intercepted signal number %d\nchild pid: %d\n", i, WTERMSIG(status), child); */
        /* } */
        /* else if (WIFSTOPPED(status)) */ 
        /* { */
        /*     printf("child %d stopped with signal number %d, child pid: %d\n", i, WSTOPSIG(status), child); */
        /* } */
    }

    /* close(fd[1]); */
    /* read(fd[0], *msg, sizeof(*msg)); */
    /* printf("parent (pid: %d) recieved msg: %s\n", getpid(), *msg); */  

    return 0;
}
