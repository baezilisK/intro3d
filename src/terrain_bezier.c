#include <math.h>
#include <stdio.h>
#include <string.h>
#include "GL/glut.h"
#include "config.h"
#include "terrain.h"
#include "util.h"

#define max(a, b) ((a) > (b) ?  (a) : (b))

/*
static void vertex (float x, float y) {
  glVertex3f (x, y, terrain_h (x, y));
}
*/

void terrain_normalgen (void) {
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

static void bezier3 (float u, float v, float i, float j, float *ret) {
  int k, l, c[] = {1, 3, 3, 1};
  ret[0] = ret[1] = ret[2] = 0;
  for (k = 0; k < 4; ++k) {
    for (l = 0; l < 4; ++l) {
      ret[0] += 
        c[k] * pow (1 - u, 3 - k) * pow (u, k) *
        c[l] * pow (1 - v, 3 - l) * pow (v, l) *
      (i + k);
      ret[1] += 
        c[k] * pow (1 - u, 3 - k) * pow (u, k) *
        c[l] * pow (1 - v, 3 - l) * pow (v, l) *
      (j + l);
      ret[2] += 
        c[k] * pow (1 - u, 3 - k) * pow (u, k) *
        c[l] * pow (1 - v, 3 - l) * pow (v, l) *
      terrain_h (i + k, j + l);
    }
  }
}

static void segment (int i0, int j0) {
  int i, j;
  float u, uu, v, vv, p[3];
  glBegin (GL_TRIANGLES);
    for (i = 1; i <= CONFIG_BEZIER_DETAIL; ++i) {
      u = (float) i / CONFIG_BEZIER_DETAIL;
      uu = (float) (i - 1) / CONFIG_BEZIER_DETAIL;
      for (j = 1; j <= CONFIG_BEZIER_DETAIL; ++j) {
        v = (float) j / CONFIG_BEZIER_DETAIL;
        vv = (float) (j - 1) / CONFIG_BEZIER_DETAIL;
        bezier3 (u, v, i0, j0, p); glVertex3fv (p);
        bezier3 (uu, v, i0, j0, p); glVertex3fv (p);
        bezier3 (uu, vv, i0, j0, p); glVertex3fv (p);
        bezier3 (uu, vv, i0, j0, p); glVertex3fv (p);
        bezier3 (u, vv, i0, j0, p); glVertex3fv (p);
        bezier3 (u, v, i0, j0, p); glVertex3fv (p);
      }
    }
  glEnd ();
}

void terrain_display (void) {
  int i, j;
  glColor3ub (0xAA, 0xAA, 0xAA);
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  for (i = 0; i + 4 <= terrain_m; i += 3) {
    for (j = 0; j + 4 <= terrain_n; j += 3) {
      segment (i, j);
    }
  }
  /*
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
  */
}

void terrain_free (void) {
  int i;
  for (i = 0; i < terrain_m; ++i)
    xfree (terrain_hmap[i]);
  xfree (terrain_hmap);
}
