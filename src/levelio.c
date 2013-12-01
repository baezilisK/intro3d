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
#include "util.h"

void levelio_read (char *fname) {
  FILE *f = fopen (fname, "r");
  if (f) {
    int i, j, n, m;
    float a, b, c;
    fscanf (f, "%f %f %f", &a, &b, &c);

    fscanf (f, "%d %d", &m, &n);
    /*terrain_hmap = xmalloc (m * sizeof *terrain_hmap); */
    for (i = 0; i < m; ++i) {
      /* terrain_hmap[i] = xmalloc (n * sizeof *terrain_hmap[i]); */
      for (j = 0; j < n; ++j) {
        fscanf (f, "%f", &terrain_hmap[i][j]);
      }
    }
    terrain_m = m; terrain_n = n;
    fclose (f);
  } else {
    levelio_errno = errno;
  }
}
