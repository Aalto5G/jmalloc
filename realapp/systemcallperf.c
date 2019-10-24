#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * This program demonstrates that system call performance is not the
 * reason for slow C malloc() performance in real-life applications.
 * Run this program with time ./systemcallperf and you get the following:
 *
 * real 0m0,001s
 * user 0m0,001s
 * sys  0m0,000s
 */

int main(int argc, char **argv)
{
  FILE *f = fopen("straceanalyzed.txt", "r");
  char *brkbase = sbrk(0);
  int brkamount = 0;
  for (;;)
  {
    if (fscanf(f, "%d", &brkamount) != 1)
    {
      break;
    }
    if (brk(brkbase + brkamount) != 0)
    {
      abort();
    }
  }
  return 0;
}
