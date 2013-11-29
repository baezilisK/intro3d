#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include "rain.h"
#include "terrain.h"

#define EPSILON 0.1

float rfg (int n) {
  return 1.0 * rand () / RAND_MAX * n;
}

void rain_mk (void) {
  if (rain_n < RAIN_N) {
    rain_x[rain_j] = rfg (terrain_mm - 1);
    rain_y[rain_j] = rfg (terrain_nn - 1);
    rain_z[rain_j] = RAIN_H;
    rain_j = (rain_j + 1) % RAIN_N;
    ++rain_n;
  }
}

void rain_tick (void) {
  int i;
  for (i = rain_i; i != rain_j; i = (i + 1) % RAIN_N)
    rain_z[i] -= RAIN_V;
  for (
      i = rain_i;
      i != rain_j && rain_z[i] < terrain_h (rain_x[i], rain_y[i]);
      i = (i + 1) % RAIN_N
  ) --rain_n;
  rain_i = i;
}

void rain_display (void) {
  int i;
  glPushMatrix ();

  for (i = rain_i; i != rain_j; i = (i + 1) % RAIN_N) {
    glColor3f (1, 1, 1);
    glBegin (GL_LINES);
      glVertex3f (rain_x[i], rain_y[i], rain_z[i]);
      glVertex3f (rain_x[i], rain_y[i], rain_z[i] - EPSILON);
    glEnd ();
  }

  glPopMatrix ();
}
