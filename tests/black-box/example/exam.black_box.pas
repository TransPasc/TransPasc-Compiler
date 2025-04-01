# Program

program Test;
var
  x, y : integer;
  flag : boolean;
begin
  x := 10;
  y := x * 2;

  if y > 15 then
  begin
    flag := true;
    while x > 0 do
      x := x - 1;
  end
  else
    flag := false;
end.

# Result
// TODO: 完成其 ast 构建
{$result-list}
