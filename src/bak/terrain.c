#include <math.h>
#include <stdio.h>
#include <string.h>
#include "GL/glut.h"
#include "config.h"
#include "terrain.h"
#include "texture.h"
#include "light.h"

static float interpolate (float a, float b, float x) {
  return b*x + a*(1 - x);
}

static float interpolate4 (
    float a, float b, float c, float d,
    float x, float y) {
  float e = interpolate (a, b, x);
  float f = interpolate (c, d, x);
  return interpolate (e, f, y);
}

static void normalize (float *v, int n) {
  float norm2 = 0;
  int i;
  for (i = 0; i < n; ++i) norm2 += v[i] * v[i];
  for (i = 0; i < n; ++i) v[i] /= sqrt (norm2);
}

static void crossproduct (float *u, float *v, float *ret) {
  ret[0] = u[1]*v[2] - u[2]*v[1];
  ret[1] = u[2]*v[0] - u[0]*v[2];
  ret[2] = u[0]*v[1] - u[1]*v[0];
}

void terrain_getnormals (void) {
  int i, j, k, h;
  float u[3], v[3], cp[3];
  int nnormal[100][100] = {{0}};

  for (i = 0; i < terrain_mm - 1; ++i) {
    for (j = 0; j < terrain_nn - 1; ++j) {
      u[0] = 1; u[1] = 0; v[0] = 0; v[1] = 1;
      u[2] = terrain_hmap[i + 1][j] - terrain_hmap[i][j];
      v[2] = terrain_hmap[i][j + 1] - terrain_hmap[i][j];
      crossproduct (u, v, cp); 
      normalize (cp, sizeof cp / sizeof *cp);
      for (h = 0; h < 3; ++h) terrain_normal[i][j][h] += cp[h];

      for (h = 0; h < 3; ++h) v[h] = -u[h];
      u[0] = 0; u[1] = 1;
      u[2] = terrain_hmap[i + 1][j + 1] - terrain_hmap[i + 1][j];
      crossproduct (u, v, cp); 
      normalize (cp, sizeof cp / sizeof *cp);
      for (h = 0; h < 3; ++h) terrain_normal[i + 1][j][h] += cp[h];

      for (h = 0; h < 3; ++h) v[h] = -u[h];
      u[0] = -1; u[1] = 0;
      u[2] = terrain_hmap[i][j + 1] - terrain_hmap[i + 1][j + 1];
      crossproduct (u, v, cp); 
      normalize (cp, sizeof cp / sizeof *cp);
      for (h = 0; h < 3; ++h) terrain_normal[i + 1][j + 1][h] += cp[h];

      for (h = 0; h < 3; ++h) v[h] = -u[h];
      u[0] = 0; u[1] = -1;
      u[2] = terrain_hmap[i][j] - terrain_hmap[i][j + 1];
      crossproduct (u, v, cp); 
      normalize (cp, sizeof cp / sizeof *cp);
      for (h = 0; h < 3; ++h) terrain_normal[i][j + 1][h] += cp[h];
      nnormal[i][j]++;
      nnormal[i + 1][j]++;
      nnormal[i+1][j+1]++;
      nnormal[i][j+1]++;
    }
  }

  for (i = 0; i < terrain_mm; ++i) {
    for (j = 0; j < terrain_nn; ++j) {
      for (k = 0; k < 3; ++k) {
        terrain_normal[i][j][k] /= nnormal[i][j];
      }
    }
  }
}

static void mkv (float x, float y, float texx, float texy) {
  float u[3] = {0}, v[4][3];
  int x0 = x, x1 = x, y0 = y, y1 = y, i;
  if (x1 + 1 < terrain_nn) ++x1; if (y1 + 1 < terrain_mm) ++y1;
  for (i = 0; i < 3; ++i) v[0][i] = terrain_normal[x0][y0][i];
  for (i = 0; i < 3; ++i) v[1][i] = terrain_normal[x1][y0][i];
  for (i = 0; i < 3; ++i) v[2][i] = terrain_normal[x0][y1][i];
  for (i = 0; i < 3; ++i) v[3][i] = terrain_normal[x1][y1][i];
  for (i = 0; i < 3; ++i)
    u[i] = interpolate4 (
      v[0][i], v[1][i], v[2][i], v[3][i],
      x - x0, y - y0
    );
  glTexCoord2f (texx, texy);
  glNormal3f (u[0], u[1], u[2]);
  glVertex3f (x, y, terrain_h (x, y));
}

#define TEXH 1.0

void terrain_display (void) {
  int i, j;
  glColor3f (1, 1, 1);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  texture_enable (
    texture_terrain, &texture_terrain_name,
    texture_terrain_dim, GL_MODULATE
  );
  glBegin (GL_TRIANGLES);
    for (i = 1; i < terrain_mm; ++i) {
      for (j = 1; j < terrain_nn; ++j) {
        mkv (i, j, TEXH, TEXH);
        mkv (i - 1, j, 0, TEXH);
        mkv (i - 0.5, j - 0.5, TEXH/2, TEXH/2);

        mkv (i - 1, j, 0, TEXH);
        mkv (i - 1, j - 1, 0, 0);
        mkv (i - 0.5, j - 0.5, TEXH/2, TEXH/2);

        mkv (i - 1, j - 1, 0, 0);
        mkv (i, j - 1, TEXH, 0);
        mkv (i - 0.5, j - 0.5, TEXH/2, TEXH/2);

        mkv (i, j - 1, TEXH, 0);
        mkv (i, j, TEXH, TEXH);
        mkv (i - 0.5, j - 0.5, TEXH/2, TEXH/2);
      }
    }
  glEnd ();

  texture_disable (&texture_terrain_name);

  /*
  glColor3f (0, 0, 1);
  glBegin (GL_QUADS);
    glNormal3f (0, 0, 1);
    glTexCoord2f (0, 0);
    glVertex3f (-CONFIG_SUN_DIST, -CONFIG_SUN_DIST, terrain_hmin - 1);
    glTexCoord2f (0, 64);
    glVertex3f (-CONFIG_SUN_DIST, CONFIG_SUN_DIST, terrain_hmin - 1);
    glTexCoord2f (64, 64);
    glVertex3f (CONFIG_SUN_DIST, CONFIG_SUN_DIST, terrain_hmin - 1);
    glTexCoord2f (64, 0);
    glVertex3f (CONFIG_SUN_DIST, -CONFIG_SUN_DIST, terrain_hmin - 1);
  glEnd ();

  */
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}

float terrain_h (float x, float y) {
  int x0, x1, y0, y1;
  float u, v;
  if (x < 0) x = 0;
  if (x > terrain_mm - 1) x = terrain_mm - 1;
  if (y < 0) y = 0;
  if (y > terrain_nn - 1) y = terrain_nn - 1;
  x0 = x, x1 = x, y0 = y, y1 = y;
  if (x + 1 < terrain_mm) ++x1;
  if (y + 1 < terrain_nn) ++y1;
  u = interpolate (terrain_hmap[x0][y0], terrain_hmap[x1][y0], x - x0);
  v = interpolate (terrain_hmap[x0][y1], terrain_hmap[x1][y1], x - x0);
  return interpolate (u, v, y - y0);
}

float *terrain_getnormal (float *u, float *ret) {
  int x0, x1, y0, y1, i;
  float a[3], b[3];
  ret[0] = 0;
  ret[1] = 0;
  ret[2] = 1;
  if (u[0] < 0 || u[0] > terrain_mm - 1) return ret;
  if (u[1] < 0 || u[1] > terrain_nn - 1) return ret;
  x0 = u[0], x1 = u[0], y0 = u[1], y1 = u[1];
  if (x1 + 1 < terrain_mm) ++x1;
  if (y1 + 1 < terrain_nn) ++y1;
  for (i = 0; i < 3; ++i)
    a[i] = interpolate (
      terrain_normal[x0][y0][i], terrain_normal[x1][y0][i], u[0] - x0
    );
  for (i = 0; i < 3; ++i)
    b[i] = interpolate (
      terrain_normal[x0][y1][i], terrain_normal[x1][y1][i], u[0] - x0
    );
  for (i = 0; i < 3; ++i)
    ret[i] = interpolate (a[i], b[i], u[1] - y0);
  return ret;
}
