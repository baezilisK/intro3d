#ifndef TERRAIN_H
#define TERRAIN_H

#define TERRAIN_N 1000

int terrain_nn, terrain_mm;
float terrain_hmap[TERRAIN_N][TERRAIN_N], terrain_hmin;
float terrain_normal[TERRAIN_N][TERRAIN_N][3];

/* interpolate normal at x */
float *terrain_getnormal (float *x, float *ret);

void terrain_getnormals (void);

void terrain_display (void);

float terrain_h (float x, float y);

#endif /* TERRAIN_H */
