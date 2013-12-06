#include <stdlib.h>
#include "sys/time.h"
#include "rng.h"

void rng_init (void) {
  struct timeval tv;
  gettimeofday (&tv, NULL);
  srand (tv.tv_sec * tv.tv_usec);
}

int rng_i (int n) {
  return rand () / (RAND_MAX/n + 1);
}

double rng_f (void) {
  return (double) rand () / RAND_MAX;
}

void rng_free (void) {
}
