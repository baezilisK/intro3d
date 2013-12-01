#include <math.h>
#include <stdio.h>
#include <string.h>
#include "GL/glut.h"
#include "config.h"
#include "terrain.h"
#include "texture.h"
#include "light.h"
#include "util.h"

static void vertex (float x, float y) {
  glVertex3f (x, y, terrain_h (x, y));
}

float terrain_h (float x, float y) {
  int x0, x1, y0, y1;
  float a, b, c;
  x = x < 0 ? 0 : x > terrain_n - 1 ? terrain_n - 1 : x;
  y = y < 0 ? 0 : y > terrain_m - 1 ? terrain_m - 1 : y;
  x0 = x; x1 = x + 1; y0 = y; y1 = y0 + 1;
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
      vertex (i, j - 1);
      vertex (i - 1, j - 1);
      vertex (i - 1, j);
      vertex (i, j - 1);
      /*
      for (u = 0; u < CONFIG_TERRAIN_DETAIL; ++u) {
        for (v = 0; v < CONFIG_TERRAIN_DETAIL; ++v) {
          vertex (i - 1 + u/p, j - 1 + v/p);
          vertex (i - 1 + (u+1)/p, j - 1 + v/p);
          vertex (i - 1 + u/p, j - 1 + (v+1)/p);
          vertex (i - 1 + (u+1)/p, j - 1 + v/p);
          vertex (i - 1 + u/p, j - 1 + (v+1)/p);
          vertex (i - 1 + (u+1)/p, j - 1 + (v+1)/p);
        }
      }
      */
    }
  }
  glEnd ();
}
