#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "GL/gl.h"
#include "config.h"
#include "road.h"
#include "terrain.h"
#include "texture.h"
#include "zutil.h"

#define len(a) (sizeof (a) / sizeof (*a))

static void normalize (double *u, int n) {
  double norm = 0;
  int i;
  for (i = 0; i < n; ++i) norm += u[i] * u[i];
  norm = sqrt (norm);
  for (i = 0; i < n; ++i) u[i] /= norm;
}

static void crossproduct (double *u, double *v, double *ret) {
  ret[0] = u[1]*v[2] - u[2]*v[1];
  ret[1] = u[2]*v[0] - u[0]*v[2];
  ret[2] = u[0]*v[1] - u[1]*v[0];
}

static float B (int i, float u) {
  float v = 1 - u;
  float ret =
    i == 0 ? v * v * v * 1 :
    i == 1 ? v * v * u * 3 :
    i == 2 ? v * u * u * 3 :
    i == 3 ? u * u * u * 1 :
  0;
  return ret;
}

static float C (float u, int j, int k, struct road r) {
  float ret = 0, *x = k ? r.sy : r.sx;
  int i;
  for (i = 0; i <= 3; ++i) {
    ret += B (i, u) * x[i + 3*j];
  }
  return ret;
}

static void road_h (int a, double t, double w, struct road r, float *x){
  double u[3], v[3], cp[3], epsilon = 0.01, up[] = {0, 0, 1};
  int i;
  u[0] = C (t, a, 0, r); u[1] = C (t, a, 1, r);
  u[2] = terrain_h (u[0], u[1]);
  v[0] = C (t + epsilon, a, 0, r); v[1] = C (t + epsilon, a, 1, r);
  v[2] = terrain_h (v[0], v[1]);
  for (i = 0; i < 3; ++i) v[i] -= u[i];
  crossproduct (v, up, cp); normalize (cp, len (cp));
  for (i = 0; i < 3; ++i) u[i] += w * cp[i];
  u[2] = terrain_h (u[0], u[1]);
  for (i = 0; i < 3; ++i) x[i] = u[i];
}

static void segment (int j, struct road r) {
  int i, k, lim = CONFIG_RENDER_DETAIL;
  for (i = 0; i < CONFIG_RENDER_DETAIL; ++i) {
    float
      a = (float)i / CONFIG_RENDER_DETAIL,
      aa = (float)(i + 1) / CONFIG_RENDER_DETAIL,
      u[3], v[3];
    for (k = 0; k < lim; ++k) {
      float b = (float)k / lim, bb = (float)(k + 1) / lim;

      road_h (j, a, b*2*r.w - r.w, r, u);
      glTexCoord2f (b, a);
      glNormal3fv (terrain_getnormal (u, v));
      glVertex3fv (u);

      road_h (j, aa, b*2*r.w - r.w, r, u);
      glTexCoord2f (b, aa);
      glNormal3fv (terrain_getnormal (u, v));
      glVertex3fv (u);

      road_h (j, aa, bb*2*r.w - r.w, r, u);
      glTexCoord2f (bb, aa);
      glNormal3fv (terrain_getnormal (u, v));
      glVertex3fv (u);

      road_h (j, a, bb*2*r.w - r.w, r, u);
      glTexCoord2f (bb, a);
      glNormal3fv (terrain_getnormal (u, v));
      glVertex3fv (u);
    }
  }
}

static void display (struct road r) {
  int i;
  texture_enable (
    texture_road, &texture_road_name,
    texture_road_dim, GL_MODULATE
  );
  glBegin (GL_QUADS);
    for (i = 0; i < r.nctrl / 3; ++i) {
      segment (i, r);
    }
  glEnd ();
  texture_disable (&texture_road_name);
}

void road_displayall (void) {
  int i;
  glColor3f (1, 1, 1);
  glPushMatrix ();
    glTranslatef (0, 0, 0.01);
    for (i = 0; i < road_n; ++i) {
      display (road_all[i]);
    }
  glPopMatrix ();
}
