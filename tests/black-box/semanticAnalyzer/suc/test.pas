program test;
var
  a: integer;
  b: integer;
  c: integer;
begin
    a := 1;
    b := 2;
    c := 3;
    if (a = b) and (b = c) then
        write('a')
    else if (a = b) or (b = c) then
        write('b')
    else if (a = c) then
        write('c')
    else if (a = b) and (b = c) and (a = c) then
        write('d')
    else if (a = b) or (b = c) or (a = c) then
        write('e')
    else if (a = 1) and (b = 2) and (c = 3) then
        write('f')
    else
        write('g');
end;
