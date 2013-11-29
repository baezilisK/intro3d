#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "levelio.h"
#include "light.h"
#include "road.h"
#include "terrain.h"
#include "tree.h"
#include "zutil.h"

#define len(arr) (sizeof(arr)/sizeof(*arr))
#define min(a, b) ((a) < (b) ? (a) : (b))

static float dotproduct (float *u, float *v, int n) {
  int i;
  float ret = 0;
  for (i = 0; i < n; ++i)
    ret += u[i] * v[i];
  return ret;
}

void levelio_read (char *fname) {
  FILE *f = fopen (fname, "r");
  if (f) {
    int i, j;
    float norm;
    fscanf (f, "%f %f %f", 
      &light_sunx[0], &light_sunx[1], &light_sunx[2]);
    norm = sqrt (dotproduct (light_sunx, light_sunx, len (light_sunx)));
    for (i = 0; i < len (light_sunx); ++i)
      light_sunx[i] *= 100 / norm;
    for (i = 0; i < 3; ++i) light_sunx[i] = -light_sunx[i];
    fscanf (f, "%d %d", &terrain_nn, &terrain_mm);
    for (i = 0; i < terrain_mm; ++i) {
      for (j = 0; j < terrain_nn; ++j) {
        fscanf (f, "%f", &terrain_hmap[j][i]);
      }
    }
    fscanf (f, "%d", &tree_n);
    tree_x = zmalloc (tree_n * sizeof *tree_x);
    tree_y = zmalloc (tree_n * sizeof *tree_y);
    for (i = 0; i < tree_n; ++i) {
      fscanf (f, "%f %f", &tree_x[i], &tree_y[i]);
    }
    fscanf (f, "%d", &road_n);
    road_all = zmalloc (road_n * sizeof *road_all);
    for (i = 0; i < road_n; ++i) {
      fscanf (f, "%d %f", &road_all[i].nctrl, &road_all[i].w);
      road_all[i].w /= 2;
      road_all[i].sx=zmalloc (road_all[i].nctrl*sizeof *road_all[i].sx);
      road_all[i].sy=zmalloc (road_all[i].nctrl*sizeof *road_all[i].sy);
      for (j = 0; j < road_all[i].nctrl; ++j) {
        fscanf (f, "%f %f", &road_all[i].sx[j], &road_all[i].sy[j]);
      }
    } 
  } else {
    levelio_errno = errno;
  }
}
