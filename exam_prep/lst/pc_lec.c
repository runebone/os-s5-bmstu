se, sf, sb: semaphore;

producer:
    while (1)
    {
        p(se);
        p(sb);
        N = N + 1;
        v(sb);
        v(sf);
    }

consumer:
    while (1)
    {
        p(sf);
        p(sb);
        N = N - 1;
        v(sb);
        v(se);
    }

begin
    se = N; sf = 0; sb = 1;
    parbegin
        producer; consumer;
    parend;
end.
