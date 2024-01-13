fp1, fp2: logical;
que: int;

p1:
    while (1):
        fp1 = 1;
        while (fp2)
        {
            if (que == 2) then
            {
                fp1 = 0;
                while (que == 2);
                fp1 = 1;
            }
        }
    CR1;
    fp1 = 0;
    que = 2;
    PR1;
end p1;

p2:
    while (1):
        fp2 = 1;
        while (fp1)
        {
            if (que == 1) then
            {
                fp2 = 0;
                while (que == 1);
                fp2 = 1;
            }
        }
    CR2;
    fp2 = 0;
    que = 1;
    PR2;
end p2;

begin
    fp1 = 0; fp2 = 0;
    que = 1;
    par begin
        p1; p2;
    par end;
end.
