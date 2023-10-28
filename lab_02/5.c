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
    for (int i = 0; i < 2; i++) 
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

int flag = 0;

void sig_handler(int sig_num)
{
    flag = 1;
}

int main(void) 
{
    pid_t child_pid[2];
    int fd[2];
    int status;
    int child;
    /* char *msg1 = "aaa\n"; */
    /* char *msg2 = "yyyyy\n"; */
    char msg[2][100] = { "aaa\n", "yyyyy\n" };
    char buf[100];
    printf("This is the test\n");
    printf("Ctrl+Z to let children write\n");
    signal(SIGTSTP, sig_handler);
    /* signal(SIGINT, sig_handler); */
    /* signal(SIGTERM, sig_handler); */
    /* signal(SIGINT, SIG_IGN); */
    /* signal(SIGSEGV, SIG_DFL); */
    sleep(2);
    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    if (pipe(fd) == -1) 
    {
        perror("cant pipe\n");
        exit(1);
    }
    for (int i = 0; i < 2; i++) 
    {
        child_pid[i] = fork();

        if (child_pid[i] == -1) 
        {
            perror("cant fork\n");
            exit(1);
        } 

        if (child_pid[i] == 0) 
        {
            if (flag)
            {
                if (i == 0)
                {
                    char str[] = "aaaa\n";
                    /* for (int i = 0; str[i]; i++) */
                    /* { */
                    /*     write(fd[1], str + i, sizeof(char)); */
                    /* } */
                    /* close(fd[0]); */
                    write(fd[1], str, sizeof(str));
                }
                else if (i == 1)
                {
                    char str[] = "yyyyyyyyyyyyyyyy\n";
                    /* for (int i = 0; str[i]; i++) */
                    /* { */
                    /*     write(fd[1], str + i, sizeof(char)); */
                    /* } */
                    /* close(fd[0]); */
                    write(fd[1], str, sizeof(str));
                }
            }
            else
            {
                printf("child %d: no signal - no message\n", i);
            }
            exit(0);
        }
        else
        {
            printf("parent pid: %d, child %d %d, group %d\n", getpid(), i, child_pid[i], getpgrp());
            /* close(fd[1]); */
            /* read(fd[0], msg[i], sizeof(*msg[i])); */
            /* read(fd[0], buf, sizeof(char)); */

            /* char ch = 0; */
            /* while (read(fd[0], &ch, sizeof(char))) */
            /* { */
            /*     printf("%c", ch); */
            /* } */

            /* do */
            /* { */
            /*     read(fd[0], &ch, sizeof(char)); */
            /*     printf("%c", ch); */
            /* } */
            /* while (ch != '\n'); */
            /* printf("parent (pid: %d) recieved msg: %s\n", getpid(), buf); */  
            exit(0);
        }
    }
    /* { */
    /*     child_pid[0] = fork(); */
    /*     if (child_pid[0] == -1) */ 
    /*     { */
    /*         perror("cant fork\n"); */
    /*         exit(1); */
    /*     } */ 
    /*     if (child_pid[0] == 0) */ 
    /*     { */
    /*         close(fd[0]); */
    /*         /1* write(fd[1], *msg1, sizeof(*msg1)); *1/ */
    /*         write(fd[1], msg[0], sizeof(*msg[0])); */
    /*         exit(0); */
    /*     } */
    /*     else */
    /*     { */
    /*         printf("parent pid: %d, child %d %d, group %d\n", getpid(), 0, child_pid[0], getpgrp()); */
    /*         close(fd[1]); */
    /*         /1* read(fd[0], *msg1, sizeof(*msg1)); *1/ */
    /*         read(fd[0], msg[0], sizeof(*msg[0])); */
    /*         /1* printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg1); *1/ */  
    /*         printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg[0]); */  
    /*     } */
    /* } */
    /* { */
    /*     child_pid[1] = fork(); */
    /*     if (child_pid[1] == -1) */ 
    /*     { */
    /*         perror("cant fork\n"); */
    /*         exit(1); */
    /*     } */ 
    /*     if (child_pid[1] == 0) */ 
    /*     { */
    /*         close(fd[0]); */
    /*         /1* write(fd[1], *msg2, sizeof(*msg2)); *1/ */
    /*         write(fd[1], msg[1], sizeof(*msg[1])); */
    /*         exit(0); */
    /*     } */
    /*     else */
    /*     { */
    /*         printf("parent pid: %d, child %d %d, group %d\n", getpid(), 1, child_pid[1], getpgrp()); */
    /*         close(fd[1]); */
    /*         /1* read(fd[0], *msg2, sizeof(*msg2)); *1/ */
    /*         read(fd[0], msg[1], sizeof(*msg[1])); */
    /*         /1* printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg2); *1/ */  
    /*         printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg[1]); */  
    /*         msg[1][0] = 0; // XXX */
    /*         /1* read(fd[0], *msg2, sizeof(*msg2)); *1/ */
    /*         read(fd[0], msg[1], sizeof(*msg[1])); */
    /*         /1* printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg2); *1/ */  
    /*         printf("parent (pid: %d) recieved msg: %s\n", getpid(), msg[1]); */  
    /*     } */
    /* } */
    for (int i = 0; i < 2; i++) 
    {
        child = wait(&status);
        /* check_macros(child_pid[i]); */

        /* close(fd[1]); */

        char ch = 0;
        while (read(fd[0], &ch, sizeof(char)))
        {
            printf("%c", ch);
        } // дома чекнуть волгу волгу

        printf("child %d exit code: %d\n", i, status);
    }
    return 0;
}
