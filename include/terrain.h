#ifndef TERRAIN_H
#define TERRAIN_H

int terrain_n, terrain_m; /* dimensions of the terrain */
float terrain_hmap[100][100];

/* interpolate the normal at x */
float *terrain_getnormal (float *x, float *ret);

/* populate the terrain_normal array based on terrain_hmap */
void terrain_getnormals (void);

/* draw the terrain */
void terrain_display (void);

/* terrain height at (x, y) */
float terrain_h (float x, float y);

#endif /* TERRAIN_H */
