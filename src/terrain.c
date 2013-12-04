#include <math.h>
#include <stdio.h>
#include <string.h>
#include "GL/glut.h"
#include "config.h"
#include "terrain.h"
#include "util.h"

#define max(a, b) ((a) > (b) ?  (a) : (b))

static float ***nmap;

static void vertex (int x, int y) {
  glNormal3fv (nmap[x][y]);
  glVertex3f (x, y, terrain_hmap[x][y]);
}

static void vertexf (float x, float y) {
  glVertex3f (x, y, terrain_h (x, y));
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

void terrain_normalgen (void) {
  int i, j, k, dim = 3;
  float 
    n[3], h, 
    up[] = {0, 1, 0}, down[] = {0, -1, 0}, 
    left[] = {-1, 0, 0}, right[] = {1, 0, 0};

  nmap = xmalloc (terrain_n * sizeof *nmap);
  for (i = 0; i < terrain_n; ++i) {
    nmap[i] = xmalloc (terrain_m * sizeof *nmap[i]);
    for (j = 0; j < terrain_m; ++j) {
      nmap[i][j] = xmalloc (dim * sizeof *nmap[i][j]);
      for (k = 0; k < dim; ++k) nmap[i][j][k] = 0;
    }
  }

  /* apologies for the repetition */
  for (i = 0; i < terrain_n; ++i) {
    for (j = 0; j < terrain_m; ++j) {
      h = terrain_hmap[i][j];
      if (i > 0) {
        left[2] = terrain_hmap[i - 1][j] - h;
        if (j > 0) {
          down[2] = terrain_hmap[i][j - 1] - h;
          crossproduct (left, down, n);
          for (k = 0; k < dim; ++k) nmap[i][j][k] += n[k];
        }
        if (j + 1 < terrain_m) {
          up[2] = terrain_hmap[i][j + 1] - h;
          crossproduct (up, left, n);
          for (k = 0; k < dim; ++k) nmap[i][j][k] += n[k];
        }
      }
      if (i + 1 < terrain_n) {
        right[2] = terrain_hmap[i + 1][j] - h;
        if (j > 0) {
          down[2] = terrain_hmap[i][j - 1] - h;
          crossproduct (down, right, n);
          for (k = 0; k < dim; ++k) nmap[i][j][k] += n[k];
        }
        if (j + 1 < terrain_m) {
          up[2] = terrain_hmap[i][j + 1] - h;
          crossproduct (right, up, n);
          for (k = 0; k < dim; ++k) nmap[i][j][k] += n[k];
        }
      }
      normalize (nmap[i][j], dim);
    }
  }
}

void terrain_display (void) {
  int i, j;
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glBegin (GL_TRIANGLES);
  glColor3ub (0xAA, 0xAA, 0xAA);
  for (i = 1; i < terrain_n; ++i) {
    for (j = 1; j < terrain_m; ++j) {
      float n[3] = {0};
      int k;
      for (k = 0; k < 3; ++k) {
        n[k] = (nmap[i][j][k] + nmap[i - 1][j][k] + 
        nmap[i][j - 1][k] + nmap[i - 1][j - 1][k]) / 4;
      }
      glNormal3fv (nmap[i][j]); vertex (i, j);
      glNormal3fv (nmap[i - 1][j]); vertex (i - 1, j);
      glNormal3fv (n); vertexf (i - 0.5, j - 0.5);
      glNormal3fv (nmap[i - 1][j]); vertex (i - 1, j);
      glNormal3fv (nmap[i - 1][j - 1]); vertex (i - 1, j - 1);
      glNormal3fv (n); vertexf (i - 0.5, j - 0.5);
      glNormal3fv (nmap[i - 1][j - 1]); vertex (i - 1, j - 1);
      glNormal3fv (nmap[i][j - 1]); vertex (i, j - 1);
      glNormal3fv (n); vertexf (i - 0.5, j - 0.5);
      glNormal3fv (nmap[i][j - 1]); vertex (i, j - 1);
      glNormal3fv (nmap[i][j]); vertex (i, j);
      glNormal3fv (n); vertexf (i - 0.5, j - 0.5);
    }
  }
  glEnd ();
}

void terrain_free (void) {
  int i, j;
  for (i = 0; i < terrain_n; ++i) {
    for (j = 0; j < terrain_m; ++j)
      xfree (nmap[i][j]);
    xfree (terrain_hmap[i]);
    xfree (nmap[i]);
  }
  xfree (terrain_hmap);
  xfree (nmap);
}
