var choosing: shared array[0..n-1] of boolean;
var number: shared array[0..n-1] of integer;
...
repeat
    choosing[i] := true;
    number[i] = max(number[0], number[1],
            ..., number[n-1]) + 1;
    choosing[i] = false;
    for j := 0 to n-1 do begin
        while choosing[i] do (*nothing*);
        while number[j] <> 0 and
            (number[j], j) < (number[i], i)
            do (*nothing*);
    end;
    (*critical section*)
    number[i] := 0;
    (*remainder section*)
until false;
