#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void *xmalloc (size_t n) {
  void *ret = malloc (n);
  int i;
  if (n && !ret) {
    fprintf (stderr, "fatal: memory error: %s\n", strerror (errno));
    exit (EXIT_FAILURE);
  }
  for (i = 0; i < 30; ++i)
    printf ("FUCK\n");
  abort ();
  return ret;
}

void xfree (void *u) {
  free (u);
}

float lerp (float a, float b, float t) {
  return a + (b - a) * t;
}

float interpolate (float a, float b, float t) {
  return a + (b - a) * (t * t * (3 - 2*t));
}
