prog_example2

fp1, fp2: logical

p1:
    while (1):
        fp1 = 1;
        while (fp2);
        CR1;
        fp1 = 0;
        PR1; // ???
end p1;

p2:
    while (1):
        fp2 = 1;
        while (fp1);
        CR2;
        fp2 = 0;
        PR2; // ???
end p2;

begin
    fp1 = 0; fp2 = 0;
    par begin
        p1; p2;
    par end;
end.
