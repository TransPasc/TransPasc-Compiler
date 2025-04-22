PROGRAM Test;
// USES
//     SysUtils;
CONST
    MAXSIZE = 100;
    MINVALUE = 0;
VAR
  x, y : integer;
  arr : array [1..2,2..3] of integer;
  flag, flag2 : boolean;
FUNCTION getMax(a, b: integer): integer;
BEGIN
  IF a > b THEN begin
    getMax := a
  END ELSE
    getMax := b;
END;

PROCEDURE printMessage(msg: char);
BEGIN
  WRITE(msg);
END;

PROCEDURE test;
VAR
  x: integer;
  y: integer;
BEGIN
// Test the getMax function
x := getMax(5, 10);

END;

BEGIN
    x := 2;
    x := 2*x + 3;
    x := -(x - 5);
    y := 20;
    y := x*y;

    // Call the procedure
    printMessage('H');

    // Array initialization
    arr[1,2] := 5;
    arr[2,3] := 10;

    printMessage('f');
    flag := x=y;
    flag2 := x<>y;

    write(x);
    write(y);
    write(arr[1,2]);
    write(arr[2,3]);
    write(flag);
    write(flag2);
END.
