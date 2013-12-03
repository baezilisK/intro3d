#include <math.h>
#include <stdio.h>
#include <string.h>
#include "GL/glut.h"
#include "config.h"
#include "terrain.h"
#include "util.h"

#define max(a, b) ((a) > (b) ?  (a) : (b))

static void vertex (float x, float y) {
  glVertex3f (x, y, terrain_h (x, y));
}

float terrain_h (float x, float y) {
  int x0, x1, y0, y1;
  float a, b, c;
  x = max (x, 0); y = max (y, 0);
  x0 = x; x1 = x0 + 1; y0 = y; y1 = y0 + 1;
  if (x >= terrain_n - 1) x0 = x1 = terrain_n - 1;
  if (y >= terrain_m - 1) y0 = y1 = terrain_m - 1;
  a = lerp (terrain_hmap[x0][y0], terrain_hmap[x1][y0], x - x0),
  b = lerp (terrain_hmap[x0][y1], terrain_hmap[x1][y1], x - x0),
  c = lerp (a, b, y - y0);
  return c;
}

void terrain_display (void) {
  int i, j;
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glBegin (GL_TRIANGLES);
  for (i = 1; i < terrain_m; ++i) {
    for (j = 1; j < terrain_n; ++j) {
      vertex (i, j);
      vertex (i - 1, j);
      vertex (i - 0.5, j - 0.5);
      vertex (i - 1, j);
      vertex (i - 1, j - 1);
      vertex (i - 0.5, j - 0.5);
      vertex (i - 1, j - 1);
      vertex (i, j - 1);
      vertex (i - 0.5, j - 0.5);
      vertex (i, j - 1);
      vertex (i, j);
      vertex (i - 0.5, j - 0.5);
    }
  }
  glEnd ();
}

void terrain_free (void) {
  int i;
  for (i = 0; i < terrain_m; ++i)
    xfree (terrain_hmap[i]);
  xfree (terrain_hmap);
}
