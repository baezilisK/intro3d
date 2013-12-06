#include <math.h>
#include "GL/glut.h"
#include "config.h"
#include "light.h"
#include "rng.h"
#include "stars.h"
#include "util.h"

#define N STARS_MAX

static int n, prox;
static float x[N], y[N], z[N];

void stars_init (int k, float r, float R) {
  int i;
  n = min (N, k);
  prox = r;
  for (i = 0; i < n; ++i) {
    float 
      p = 2*PI*rng_f (), q = 2*PI*rng_f (),
      rr = r + (R - r) * rng_f ();
    x[i] = rr * cos (p) * cos (q);
    y[i] = rr * sin (p) * cos (q);
    z[i] = rr * sin (q);
  }
}

static void stars (void) {
  int i = 0;
  glPointSize (1.5);
  glColor3f (1, 1, 1);
  glBegin (GL_POINTS);
    for (i = 0; i < n; ++i) {
      glVertex3f (x[i], y[i], z[i]);
    }
  glEnd ();
  glPointSize (1);
}

static void sun (void) {
  float r = prox / sqrt (dotproduct (light_sunx, light_sunx, 3));
  glPushMatrix ();
    glTranslatef (r*light_sunx[0], r*light_sunx[1], r*light_sunx[2]);
    glutWireSphere (
      CONFIG_SUN_RADIUS,
      CONFIG_RENDER_DETAIL, CONFIG_RENDER_DETAIL
    );
  glPopMatrix ();
}

void stars_display (void) {
  if (stars_daytime) sun ();
  else stars ();
}
