monitor: resource;
var
    busy: logical;
    x: conditional;

procedure acquire;
    begin
        if busy then wait(x);
        busy := true;
    end;

procedure release;
    begin
        busy := false;
        signal(x);
    end;

begin
    busy := false;
end.
