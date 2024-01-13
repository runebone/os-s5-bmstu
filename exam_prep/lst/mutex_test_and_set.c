program use_test_and_set;
flag, c1, c2: logical;

p1:
    while (1)
    {
        c1 = 1;
        while (c1 == 1)
            test_and_set(c1, flag);
        CR1;
        flag = 0;
        PR1;
    }

p2:
    while (1)
    {
        c2 = 1;
        while (c2 == 1)
            test_and_set(c2, flag);
        CR2;
        flag = 0;
        PR2;
    }

main()
{
    flag = 0;
    parbegin
        p1; p2;
    parend;
}
