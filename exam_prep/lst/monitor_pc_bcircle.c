monitor: resource;
var
    bcircle: array[0..n-1] of type;
    pos: 0..n; // текущая позиция
    j: 0..n; // заполняемая позиция
    k: 0..n; // освобождамая позиция
    buffer_full, buffer_empty: conditional;

procedure producer(var data: type);
begin
    if pos = n then
        wait(buffer_empty);
    bcircle[j] := data;
    pos := pos + 1;
    j := (j + 1) mod n;
    signal(buffer_full);
end;

procedure consumer(var data: type);
begin
    if (pos = 0) then
        wait(buffer_full);
    data := bcircle[k];
    pos := pos - 1;
    k := (k + 1) mod n;
    signal(buffer_empty);
end;

begin
    pos := 0;
    j := 0;
    k := 0;
end.
