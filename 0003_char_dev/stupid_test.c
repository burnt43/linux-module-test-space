#include<stdio.h>

int main () {
  int i = 0;
  int j = 0;

  while ( i < 10 && j < 10 ) {
    printf("(%d,%d)\n",i++,j++);
  }
  return 0;
}
