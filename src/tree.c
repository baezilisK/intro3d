#include <math.h>
#include "GL/glut.h"
#include "config.h"
#include "terrain.h"
#include "texture.h"
#include "tree.h"

#define PI 3.14159265358979323846
#define PHI ((1 + sqrt (5)) / 2)
#define TRUNK_HEIGHT (1 + LEAF_RADIUS)
#define TRUNK_RADIUS (1 / PI)
#define LEAF_RADIUS (1 / PHI)
#define LEAF_OFFSET TRUNK_HEIGHT

static void trunk (float x0, float y0) {
  int i;
  float t, x, y, r = TRUNK_RADIUS, h;
  texture_enable (
    texture_tree, &texture_tree_name,
    texture_tree_dim, GL_MODULATE
  );
  glBegin (GL_QUAD_STRIP);
    for (i = 0; i <= CONFIG_TREE_DETAIL; ++i) {
      t = 2 * PI * i / CONFIG_TREE_DETAIL;
      x = r * cos (t); y = r * sin (t);
      h = terrain_h (x + x0, y + y0);
      glNormal3f (x, y, 0);
      glTexCoord2f (t / (2 * PI), 0);
      glNormal3f (x/r, y/r, 0);
      glVertex3f (x + x0, y + y0, h);
      glTexCoord2f (t / (2 * PI), 1);
      glNormal3f (x/r, y/r, 0);
      glVertex3f (x + x0, y + y0, h + TRUNK_HEIGHT);
    }
  glEnd ();
  texture_disable (&texture_tree_name);
}

static void leaves (void) {
  GLUquadric *a = gluNewQuadric ();
  texture_enable (
    texture_leaf, &texture_leaf_name,
    texture_leaf_dim, GL_MODULATE
  );
  gluQuadricTexture (a, GL_TRUE);
  gluSphere (a, 0.8, CONFIG_RENDER_DETAIL, CONFIG_RENDER_DETAIL);
  gluDeleteQuadric (a);
  texture_disable (&texture_tree_name);
}

static void tree_display (float x, float y) {
  glColor3f (1, 1, 1); trunk (x, y);
  glPushMatrix ();
    glTranslatef (x, y, terrain_h (x, y) + LEAF_OFFSET);
    glColor3f (0, 1, 0); leaves ();
  glPopMatrix ();
}

void tree_displayall (void) {
  int i;
  for (i = 0; i < tree_n; ++i) {
    tree_display (tree_x[i], tree_y[i]);
  }
}
