#include "stdio.h"
int checkCPU ()
{
  union w
  {
    int a;
    char b;
  }c;
  c.a=1;
  return (c.b==1);
}
int main(void){
    int n=1;
    printf(*(char *)&n ? "l\n" : "b\n");
    return 0;
}
