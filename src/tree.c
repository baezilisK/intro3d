#include <math.h>
#include "GL/glut.h"

#include "config.h"
#include "terrain.h"
#include "tree.h"
#include "util.h"

static void trunk (float x, float y) {
  int i;
  float u, v, t, r = CONFIG_TREE_R, z;
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glBegin (GL_QUAD_STRIP);
    for (i = 0; i <= CONFIG_RENDER_DETAIL; ++i) {
      t = 2 * PI * i / CONFIG_RENDER_DETAIL;
      u = x + r * cos (t); v = y + r * sin (t);
      z = terrain_h (u, v);
      glVertex3f (u, v, z);
      glVertex3f (u, v, z + CONFIG_TREE_H);
    }
  glEnd ();
}

static void leaves (float x, float y) {
  (void) x; (void) y;
}

static void display (float x, float y) {
  glColor3ub (0x96, 0x4B, 0x00); trunk (x, y);
  glColor3ub (20, 188, 42); leaves (x, y);
}

void tree_displayall (void) {
  int i;
  for (i = 0; i < tree_n; ++i) {
    display (tree_x[i], tree_y[i]);
  }
}

void tree_free (void) {
  xfree (tree_x);
  xfree (tree_y);
}
