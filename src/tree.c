#include <assert.h>
#include <math.h>
#include "GL/glut.h"

#include "config.h"
#include "terrain.h"
#include "texture.h"
#include "tree.h"
#include "util.h"

static void trunk (float x, float y, float z) {
  int i;
  float u, v, t, ct, st, r = CONFIG_TREE_R;
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

static void leaves (float x, float y, float z) {
  GLUquadric *a = gluNewQuadric (); assert (a);
  glPushMatrix ();
    glTranslatef (x, y, z + CONFIG_TREE_H);
    texture_enable (TEXTURE_LEAF);
    gluQuadricTexture (a, GL_TRUE);
    gluSphere (
      a, 3*CONFIG_TREE_R,
      CONFIG_RENDER_DETAIL, CONFIG_RENDER_DETAIL
    );
    texture_disable ();
  glPopMatrix ();
  gluDeleteQuadric (a);
}

static void display (float x, float y) {
  float z = terrain_h (x, y);
  glColor3f (1, 1, 1);
  trunk (x, y, z);
  leaves (x, y, z);
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
