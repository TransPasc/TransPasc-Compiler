program TestParser;

(* Variable Declaration *)
var
  x, y, z: integer;
  flag: boolean;
  ch: char;
  arr: array [1..10] of real;

(* Procedure Definition *)
procedure PrintSum(a, b: integer);
var
  sum: integer;
begin
  sum := a + b;
  write(sum);
end

(* Function Definition *)
function IsPositive(num: integer): boolean;
begin
  if num > 0 then
    IsPositive := true
  else
    IsPositive := false;
end

(* Main Program *)
begin
  (* Simple Assignment *)
  x := 10;
  y := 20;
  z := x * y - 5;

  (* Conditional Statement *)
  if z > 100 then
    flag := true
  else
    flag := false;

  (* Loop Statement - Replaced While with For *)
  for x := 10 to 14 do
    write(x);

  (* Procedure Call *)
  PrintSum(5, 7);

  (* Function Call *)
  if IsPositive(z) then
    write(z)
  else
    write(-z);

  (* Array Operation *)
  arr[1] := 3.14;
  arr[2] := 2.718;

  (* Character Operation *)
  ch := 'A';
  write(ch);
end.