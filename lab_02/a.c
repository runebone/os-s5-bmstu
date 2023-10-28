#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define N 2

int main(int argc, char *argv[])
{
    pid_t childpid[N];

    for (int i = 0; i < N; i++)
    {
        childpid[i] = fork();

        if (childpid[i] == -1)
        {
            perror("Can't fork child\n");
        }
        else if (childpid[i] == 0)
        {
            printf("B child %d: pid = %d, ppid = %d, pppid = %d, pgrp = %d\n",
                    i, childpid[i], getpid(), getppid(), getpgrp());

            sleep(2);

            printf("E child %d: pid = %d, ppid = %d, pppid = %d, pgrp = %d\n",
                    i, childpid[i], getpid(), getppid(), getpgrp());
        }
        else
        {
            printf("Parent code %d\n", i);
        }
    }

    printf("E parent: pid = %d, ppid = %d\n", getpid(), getppid());

    return 0;
}
