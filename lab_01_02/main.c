#include <stdio.h>

int main()
{
    int childpid;

    if ((childpid = fork()) == -1)
    {
        perror("Can't fork.\n");
        return 1;
    }
    else if (childpid == 0)
    {
        while (1) printf("A %d\n", getpid());
        return 0;
    }
    else
    {
        while (1) printf("B %d\n", getpid());
        return 0;
    }
}
