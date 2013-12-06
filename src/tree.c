#include <math.h>
#include "GL/glut.h"

#include "config.h"
#include "terrain.h"
#include "texture.h"
#include "tree.h"
#include "util.h"

static void trunk (float x, float y) {
  int i;
  float u, v, t, ct, st, r = CONFIG_TREE_R, z;
  texture_enable (TEXTURE_TREE);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glBegin (GL_QUAD_STRIP);
    for (i = 0; i <= CONFIG_RENDER_DETAIL; ++i) {
      t = 2 * PI * i / CONFIG_RENDER_DETAIL;
      ct = cos (t); st = sin (t);
      u = x + r * ct; v = y + r * st;
      z = terrain_h (u, v);
      glNormal3f (ct, st, 0);
      glTexCoord2f (0, t); glVertex3f (u, v, z);
      glTexCoord2f (1, t); glVertex3f (u, v, z + CONFIG_TREE_H);
    }
  glEnd ();
  texture_disable ();
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
  glEnable (GL_POLYGON_OFFSET_FILL);
  glPolygonOffset (-1, -1);
  for (i = 0; i < tree_n; ++i)
    display (tree_x[i], tree_y[i]);
  glDisable (GL_POLYGON_OFFSET_FILL);
}

void tree_free (void) {
  xfree (tree_x);
  xfree (tree_y);
}
