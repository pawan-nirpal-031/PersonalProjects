#include "memory.h"

void *memset(void *ptr, int c, size_t size) {
  char *Cptr = (char *)ptr;
  for (int i = 0; i < size; i++)
    Cptr[i] = (char)c;
  return ptr;
}