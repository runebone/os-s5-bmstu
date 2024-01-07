#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>

int main()
{
    int fd;
    struct input_event ev;
    fd = open("/dev/input/event0", O_RDONLY);

    if (fd == -1)
    {
        perror("Не удалось открыть устройство");
        return 1;
    }

    while (1)
    {
        read(fd, &ev, sizeof(struct input_event));
        if (ev.type == EV_KEY)
        {
            printf("Код клавиши: %d Состояние: %d\n", ev.code, ev.value);
        }
    }

    close(fd);
    return 0;
}
