monitor: resource;
var
    nr: integer; // количество читателей
    wrt: logical; // активный писатель
    c_read, c_write: conditional; // can_read, can_write

procedure startread;
begin
    if wrt or turn(c_write)
        then wait(c_read);
    nr := nr + 1;
    signal(c_read);
end;
procedure stopread;
begin
    nr := nr - 1;
    if nr = 0 then
        signal(c_write);
end;

procedure startwrite;
begin
    if nr > 0 or wrt then
        wait(c_write);
    wrt := true;
end;
procedure stopwrite;
begin
    wrt := false;
    if turn(c_read) then
        signal(c_read);
    else
        signal(c_write);
end;

begin
    nr := 0;
    wrt := false;
end.
