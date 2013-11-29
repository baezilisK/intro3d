#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main (void) {
  int c;
  for (c = 0; c <= UCHAR_MAX; ++c) {
    printf ("%c", c);
  }
  exit (EXIT_SUCCESS);
}
