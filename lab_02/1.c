#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define N 2
#define SLEEP_FOR 2

int main()
{
    pid_t child_pid[N];

    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());

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
            sleep(SLEEP_FOR);
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
   
    return 0;
}
