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

void util_rprism (
    float x0, float x1,
    float y0, float y1,
    float z0, float z1) {
  float vx[][4] = {
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 1, 1, 0},
    {0, 1, 1, 0}
  };
  float vy[][4] = {
    {0, 0, 1, 1},
    {0, 0, 1, 1},
    {0, 0, 1, 1},
    {0, 0, 1, 1},
    {0, 0, 0, 0},
    {1, 1, 1, 1}
  };
  float vz[][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 0, 1, 1}
  };
  int i, j;
  glBegin (GL_QUADS);
    for (i = 0; i < 6; ++i)
      for (j = 0; j < 4; ++j) {
        glVertex3f (vx[i][j]?x0:x1, vy[i][j]?y0:y1, vz[i][j]?z0:z1);
      }
  glEnd ();
}
