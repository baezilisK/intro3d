#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "camera.h"
#include "config.h"
#include "light.h"
#include "texture.h"
#include "GL/glut.h"

#define len(a) (sizeof (a) / sizeof *(a))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define PI 3.14159265358979323846

static int vertical;

int feq (float a, float b) {
  return fabs (a - b) < FLT_EPSILON;
}

void light_enable (void) {
  const float ra = 0.05, rd = 0.8, rs = 0.5, ga = 0;
  float
    ambient[4] = {1, 1, 1, 1},
    diffuse[4] = {1, 1, 1, 1},
    specular[4] = {1, 1, 1, 1},
    gambient[4] = {1, 1, 1, 1},

    torchambient[4],
    torchdiffuse[4],
    torchspecular[4],
    torchpos[] = {0, 0, 0, 1};
  int i;
  for (i = 0; i < 3; ++i) {
    ambient[i] = ra;
    diffuse[i] = rd;
    specular[i] = rs;
    gambient[i] = ga;
    torchambient[i] = ra;
    torchdiffuse[i] = i == 2 ? 0.3 : 0.6;
    torchspecular[i] = 0.5;
  }
  if (!vertical && config_daynight)
    for (i = 0; i < 3; ++i)
      diffuse[i] = max (ra, sin (light_suntheta));
  glLightfv (GL_LIGHT0, GL_POSITION, light_sunx);
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv (GL_LIGHT1, GL_AMBIENT, torchambient);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, torchdiffuse);
  glLightfv (GL_LIGHT1, GL_SPECULAR, torchspecular);
  glLightfv (GL_LIGHT1, GL_POSITION, torchpos);
  glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
  glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
  glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, gambient);
  glEnable (GL_LIGHTING);
  glEnable (GL_COLOR_MATERIAL);
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHT1);
}

static void crossproduct (float *u, float *v, float *ret) {
  ret[0] = u[1]*v[2] - u[2]*v[1];
  ret[1] = u[2]*v[0] - u[0]*v[2];
  ret[2] = u[0]*v[1] - u[1]*v[0];
}

static float dotproduct (float *u, float *v) {
  float ret = 0;
  int i;
  for (i = 0; i < 3; ++i) ret += u[i] * v[i];
  return ret;
}

static float norm (float *u) {
  return sqrt (dotproduct (u, u));
}

float light_sunangle (void) {
  float *a = light_sunx, b[3] = {0}, cost;
  b[0] = light_sunx[0]; b[1] = light_sunx[1];
  if (b[0] == 0 && b[1] == 0)
    return PI / 2;
  else 
    cost = dotproduct (a, b) / (norm (a) * norm (b));
  return acos (cost);
}

void light_drawtorch (void) {
  float x[3], dx[3];
  double xx[3];
  x[0] = camera_x; x[1] = camera_y; x[2] = camera_z;
  camera_getdx (xx);
  dx[0] = xx[0]; dx[1] = xx[1]; dx[2] = xx[2];
  if (config_thirdperson) {
    x[0] += dx[0]; x[1] += dx[1]; x[2] += dx[2];
  }
  glLightfv (GL_LIGHT1, GL_POSITION, x);
  glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, dx);
}

void light_drawsun (void) {
  float bright[] = {1, 1, 1, 1}, prev[4], u[3], cp[3], v[3] = {0, 0, 1};
  float zero[] = {1, 1, 1};
  GLUquadric *a;
  int i;
  cp[0] = cp[1] = cp[2] = 1;
  for (i = 0; i < len (u); ++i) u[i] = light_sunx[i];
  if (feq (light_sunx[0], 0) && feq (light_sunx[1], 0)) vertical = 1;
  else crossproduct (u, v, cp);
  glColor3f (1, vertical? 1 : sin (light_suntheta), 0);
  texture_enable (
    texture_sun, &texture_sun_name, 
    texture_sun_dim, GL_MODULATE
  );
  glPushMatrix ();
    if (!vertical) {
      glRotatef (light_suntheta * 180 / PI, cp[0], cp[1], cp[2]);
      glTranslatef (u[0], u[1], 0);
    } else glTranslatef (u[0], u[1], u[2]);
    glLightfv (GL_LIGHT0, GL_POSITION, zero);
    a = gluNewQuadric ();
    gluQuadricTexture (a, GL_TRUE);
    glGetLightfv (GL_LIGHT0, GL_AMBIENT, prev);
    glLightfv (GL_LIGHT0, GL_AMBIENT, bright);
    gluSphere (
      a, CONFIG_SUN_RADIUS, 
      CONFIG_RENDER_DETAIL, CONFIG_RENDER_DETAIL
    );
    glLightfv (GL_LIGHT0, GL_AMBIENT, prev);
    gluDeleteQuadric (a);
  glPopMatrix ();
  texture_disable (&texture_sun_name);
}
