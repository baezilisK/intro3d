#include <math.h>
#include "GL/glut.h"
#include "rng.h"
#include "stars.h"
#include "util.h"

#define N STARS_MAX

int n;
float x[N], y[N], z[N];

void stars_init (int k, float r, float R) {
  int i;
  n = min (N, k);
  for (i = 0; i < n; ++i) {
    float 
      p = 2*PI*rng_f (), q = 2*PI*rng_f (),
      rr = r + (R - r) * rng_f ();
    x[i] = rr * cos (p) * cos (q);
    y[i] = rr * sin (p) * cos (q);
    z[i] = rr * sin (q);
  }
}

void stars_display (void) {
  int i = 0;
  glPointSize (1.5);
  glColor3f (1, 1, 1);
  glBegin (GL_POINTS);
    for (i = 0; i < n; ++i) {
      glVertex3f (x[i], y[i], z[i]);
    }
  glEnd ();
  glPointSize (1);
}
