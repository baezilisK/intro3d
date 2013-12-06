#include <stdio.h>
#include "GL/gl.h"
#include "rain.h"
#include "rng.h"
#include "util.h"

#define N RAIN_MAX
#define H 10
#define V 0.3
#define EPSILON 0.1

#define INC(a) ((a) = ((a) + 1) % N)

static int n, a, b;
static float x[N], y[N], z[N];

void rain_mk (int k, float x0, float x1, float y0, float y1) {
  int i;
  k = min (k, N - n);
  for (i = 0; i < k; ++i) {
    x[b] = x0 + (x1 - x0) * rng_f ();
    y[b] = y0 + (y1 - y0) * rng_f ();
    z[b] = H;
    INC (b);
    ++n;
  }
}

void rain_tick (void) {
  int i;
  for (i = a; i != b; INC (i)) z[i] -= V;
  for (i = a; i != b && z[i] < 0; INC(i)) --n;
  a = i;
}

void rain_display (void) {
  int i;
  glColor3f (1, 1, 1);
  glBegin (GL_LINES);
    for (i = a; i != b; INC (i)) {
      glVertex3f (x[i], y[i], z[i]);
      glVertex3f (x[i], y[i], z[i] - EPSILON);
    }
  glEnd ();
}
