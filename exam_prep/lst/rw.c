#define SHMSIZE sizeof(int)
#define PERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#define NW  3
#define NR  5

#define R_DELAY 1
#define W_DELAY 1

#define C_WAITING_R 0
#define C_ACTIVE_R 1
#define C_WAITING_W 2
#define B_ACTIVE_W 3

struct sembuf sem_start_read[] = {
    { C_WAITING_R,  1, 0 },
    { B_ACTIVE_W,   0, 0 },
    { C_WAITING_W,  0, 0 },
    { C_WAITING_R, -1, 0 },
    { C_ACTIVE_R,   1, 0 },
};

struct sembuf sem_stop_read[] = {
    { C_ACTIVE_R,  -1, 0 },
};

struct sembuf sem_start_write[] = {
    { C_WAITING_W,  1, 0 },
    { C_ACTIVE_R,   0, 0 },
    { B_ACTIVE_W,  -1, 0 },
    { C_WAITING_W, -1, 0 },
};

struct sembuf sem_stop_write[] = {
    { B_ACTIVE_W,   1, 0 },
};

int semid;
int flag = 1;

void sig_handler(int sig_num)
{
    flag = 0;
    printf("pid: %d, signal catch: %d\n", getpid(), sig_num);
}

int start_read(const int semid)
{
    return semop(semid, sem_start_read, 5);
}

int stop_read(const int semid)
{
    return semop(semid, sem_stop_read, 1);
}

int start_write(const int semid)
{
    return semop(semid, sem_start_write, 4);
}

int stop_write(const int semid)
{
    return semop(semid, sem_stop_write, 1);
}

void reader(const int semid, const char *shm)
{
    srand(getpid());
    printf("R[%5d] created.\n", getpid());
    while (flag)
    {
        usleep((double)rand() / RAND_MAX * 1000000 * R_DELAY);
        if (start_read(semid) == -1)
        {
            perror("start read error\n");
            exit(1);
        }
        // Critical section begin
		printf("R[%5d]: %3d\n", getpid(), *((int*)shm));
        // Critical section end
        if (stop_read(semid) == -1)
        {
            perror("stop read error\n");
            exit(1);
        }
    }
    exit(0);
}

void writer(const int semid, char *shm)
{
    srand(getpid());
    printf("W[%5d] created.\n", getpid());
    while (flag)
    {
        usleep((double)rand() / RAND_MAX * 1000000 * W_DELAY);
        if (start_write(semid) == -1)
        {
            perror("start write error\n");
            exit(1);
        }
        // Critical section begin
        ++(*(int*)shm);
		printf("W[%5d]: %3d\n", getpid(), *((int*)shm));
        // Critical section end
        if (stop_write(semid) == -1)
        {
            perror("stop write error\n");
            exit(1);
        }
    }
    exit(0);
}

int main()
{
    signal(SIGINT, sig_handler);

    pid_t pids[NW + NR];

    int memkey = 0;
    int shmid = shmget(memkey, SHMSIZE, IPC_CREAT | PERMS);
    if (shmid == -1) { perror("shmget\n"); exit(1); }

    char *shmaddr = (char*)shmat(shmid, NULL, 0);
    if (shmaddr == (char*)-1) { perror("shmat\n"); exit(1); }

    memset(shmaddr, 0, SHMSIZE);

    int semkey = ftok("keyfile", 0);
	if ((semid = semget(semkey, 4, IPC_CREAT | PERMS)) == -1)
    {
        perror("semget\n");
        exit(1);
    }

    int cbsaw = semctl(semid, B_ACTIVE_W, SETVAL, 1);

	if (cbsaw == -1)
    {
        perror("semctl\n");
        exit(1);
    }

    pid_t pid = -1;
    for (int i = 0; i < NW; i++)
    {
        pid = fork();
        if (pid == -1) { perror("w can't fork\n"); exit(1); }
        if (pid == 0) { writer(semid, shmaddr); }
        else { pids[i] = pid; }
    }
    for (int i = 0; i < NR; i++)
    {
        pid = fork();
        if (pid == -1) { perror("r can't fork\n"); exit(1); }
        if (pid == 0) { reader(semid, shmaddr); }
        else { pids[NW + i] = pid; }
    }

	for (int i = 0; i < (NW + NR); i++) 
    {
        check_macros(pids[i]);
    }

    if (shmdt(shmaddr) == -1) { perror("shmdt\n"); exit(1); }

    if (shmctl(shmid, IPC_RMID, (void*)shmaddr) < 0)
    {
        perror("rm shm error\n");
        exit(1);
    }

	if (semctl(semid, 0, IPC_RMID) < 0)
    {
        perror("rm sem error\n");
        exit(1);
    }
}
