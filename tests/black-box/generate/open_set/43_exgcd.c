// Generated C code by Pascal Compiler
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
; // no const decls
// var decls
int x[1];
int y[1];
int a;
int b;
// subprogram decls
int exgcd(int a, int b, int *x, int *y) {
  // no const decls
  // var decls
  int t;
  int r;
  {
    if ((b == 0)) {
      {
        {
          *x = 1;
          ;
          *y = 0;
          ;
          return a;
          ;
          // null
        }
      }
    } else {
      {
        {
          r = exgcd(b, a % b, &*x, &*y);
          ;
          t = *x;
          ;
          *x = *y;
          ;
          *y = (t - (a / b) * *y);
          ;
          return r;
          ;
          // null
        }
      }
    };
    // null
  }
}
int main() {
  {
    a = 7;
    ;
    b = 15;
    ;
    x[0] = 1;
    ;
    y[0] = 1;
    ;
    /* procedure call */
    exgcd(a, b, &x[0], &y[0]);
    ;
    ;
    x[0] = ((x[0] % b) + b) % b;
    ;
    printf("%d", x[0]);
    ;
    // null
  }
}
