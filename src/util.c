#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GL/gl.h"
#include "util.h"

void *xmalloc (size_t n) {
  void *ret = malloc (n);
  if (n && !ret) {
    fprintf (stderr, "fatal: memory error: %s\n", strerror (errno));
    exit (EXIT_FAILURE);
  }
  return ret;
}

void xfree (void *u) {
  free (u);
}

void crossproduct (float *u, float *v, float *ret) {
  ret[0] = u[1]*v[2] - u[2]*v[1];
  ret[1] = u[2]*v[0] - u[0]*v[2];
  ret[2] = u[0]*v[1] - u[1]*v[0];
}

float dotproduct (float *u, float *v, int dim) {
  int i;
  float ret = 0;
  for (i = 0; i < dim; ++i)
    ret += u[i] * v[i];
  return ret;
}

void normalize (float *u, int dim) {
  int i;
  float j = 0;
  for (i = 0; i < dim; ++i) j += u[i] * u[i];
  j = sqrt (j);
  for (i = 0; i < dim; ++i) u[i] /= j;
}

float lerp (float a, float b, float t) {
  return a + (b - a) * t;
}

float interpolate (float a, float b, float t) {
  return a + (b - a) * (t * t * (3 - 2*t));
}
