#include <stdio.h>
#include <string.h>
#include "config.h"
#include "levelio.h"
#include "light.h"
#include "road.h"
#include "terrain.h"
#include "tree.h"
#include "util.h"

static FILE *f;

static void sun (void) {
  fscanf (f, "%f %f %f", light_sunx, light_sunx+1, light_sunx+2);
}

static void terrain (void) {
  int i, j, m, n;
  fscanf (f, "%d %d", &m, &n);
  terrain_hmap = xmalloc (n * sizeof *terrain_hmap);
  for (i = 0; i < n; ++i)
    terrain_hmap[i] = xmalloc (m * sizeof *terrain_hmap[i]);
  /* note the input comes in rows */ for (j = 0; j < m; ++j)
    for (i = 0; i < n; ++i)
      fscanf (f, "%f", &terrain_hmap[i][j]);
  terrain_m = m; terrain_n = n;
}

static void trees (void) {
  int i;
  fscanf (f, "%d", &tree_n);
  tree_x = xmalloc (tree_n * sizeof *tree_x);
  tree_y = xmalloc (tree_n * sizeof *tree_y);
  for (i = 0; i < tree_n; ++i) {
    fscanf (f, "%f %f", &tree_x[i], &tree_y[i]);
  }
}

static void roads (void) {
  int i, j;
  fscanf (f, "%d\n", &road_n);
  road_all = xmalloc (road_n * sizeof *road_all);
  for (i = 0; i < road_n; ++i) {
    int *n = &road_all[i].nctrl;
    float 
      *w = &road_all[i].w, 
      **x = &road_all[i].cx, **y = &road_all[i].cy;
    fscanf (f, "%d %f\n", n, w);
    *x = xmalloc (*n * sizeof **x);
    *y = xmalloc (*n * sizeof **y);
    for (j = 0; j < road_all[i].nctrl; ++j) {
      fscanf (f, "%f %f", *x + j, *y + j);
    }
  }
}

int levelio_read (char *fname) {
  f = fopen (fname, "r");
  if (f) {
    sun ();
    terrain ();
    trees ();
    roads ();
    fclose (f);
  }
  return !f;
}
