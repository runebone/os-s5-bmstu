var
    forks: array[1..5] of semaphore;
    i: integer;

begin
    repeat
        forks[i] := 1;
        i := i - 1;
    until i = 0;

parbegin
    1: begin
        left := 1; right := 2;
    end;
    2: begin
        left := 2; right := 3
    end;
    ...
    5: begin
        left := 5; right := 1
        repeat
            // размышляет
            p(forks[left], forks[right]); // освобождена
            // ест
            v(forks[left], forks[right]); // захват
        forever;
    end; // 5
parend;

end.
