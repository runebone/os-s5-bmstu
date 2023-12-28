struct client_data
{
    int pid;
    int ticket_number;
};

struct service_data
{
    int served_by_pid;
    int served_by_tid;
    char bun_name[255];
    hyper service_time_ms;
};

program BAKERY_PROG
{
    version BAKERY_VERS
    {
        client_data get_ticket(client_data*) = 1;
        service_data get_service(client_data*) = 2;
    } = 1;
} = 0x21234567;
