#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "GL/gl.h"

#include "config.h"
#include "road.h"
#include "terrain.h"
#include "util.h"

static void bezier3 (float t, float x[4], float y[4], float ret[2]) {
  int i, j, c[] = {1, 3, 3, 1};
  for (i = 0; i < 2; ++i) {
    float *p = i ? y : x;
    ret[i] = 0;
    for (j = 0; j < 4; ++j) {
      ret[i] += c[j] * pow (1 - t, 3 - j) * pow (t, j) * p[j];
    }
  }
}

static void segment (float w, float *x, float *y) {
  int i, j;
  float t, p[3], q[3], up[3] = {0, 0, 1}, m[3], epsilon = 0.01;
  p[0] = x[0]; p[1] = y[1];
  glBegin (GL_QUAD_STRIP);
    for (i = 0; i <= CONFIG_RENDER_DETAIL; ++i) {
      t = (float) i / CONFIG_RENDER_DETAIL;
      bezier3 (t, x, y, p);
      bezier3 (t + epsilon, x, y, q);
      p[2] = terrain_h (p[0], p[1]);
      q[2] = terrain_h (q[0], q[1]);
      for (j = 0; j < len (q); ++j) q[j] -= p[j];
      crossproduct (q, up, m);
      normalize (m, len (m));

      for (j = 0; j < len (m); ++j) p[j] -= w * m[j]; glVertex3fv (p);
      for (j = 0; j < len (m); ++j) p[j] += 2*w * m[j]; glVertex3fv (p);

    }
  glEnd ();
}

static void display (struct road r) {
  int i;
  assert ((r.nctrl - 1) % 3 == 0);
  for (i = 0; i+4 <= r.nctrl; i += 3) {
    segment (r.w, r.cx + i, r.cy + i);
  }
}

void road_displayall (void) {
  int i;
  glColor3f (1, 0, 0);
  for (i = 0; i < road_n; ++i)
    display (road_all[i]);
}

void road_free (void) {
  int i;
  for (i = 0; i < road_n; ++i) {
    xfree (road_all[i].cx);
    xfree (road_all[i].cy);
  }
  xfree (road_all);
}
