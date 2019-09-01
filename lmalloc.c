#include "linkedlist.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void *volatile ptr;

int main(int argc, char **argv)
{
  size_t i;
  for (i = 0; i < 100*1000*1000; i++)
  {
    size_t sz = 1<<(4 + rand()%4);
    ptr = malloc(sz);
    free(ptr);
    ptr = malloc(sz<<1);
    free(ptr);
    ptr = malloc(sz<<2);
    free(ptr);
    ptr = malloc(sz<<3);
    free(ptr);
    ptr = malloc(sz<<4);
    free(ptr);
    ptr = malloc(sz);
    free(ptr);
    ptr = malloc(sz<<1);
    free(ptr);
    ptr = malloc(sz<<2);
    free(ptr);
    ptr = malloc(sz<<3);
    free(ptr);
    ptr = malloc(sz<<4);
    free(ptr);
  }
}
