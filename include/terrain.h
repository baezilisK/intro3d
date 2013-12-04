#ifndef TERRAIN_H
#define TERRAIN_H

int terrain_n, terrain_m; /* dimensions of the terrain */
float **terrain_hmap; /* 
                       * terrain_hmap[i][j] refers to the height of the
                       * terrain at (i, j)
                       */

/*
 * to be called after terrain_hmap[terrain_m][terrain_n] is populated,
 * this calculates normals based on the heightmap
 */
void terrain_normalgen (void);

/* interpolate the normal at x */
/*
float *terrain_getnormal (float *x, float *ret);
*/

/* populate the terrain_normal array based on terrain_hmap */
void terrain_getnormals (void);

/* draw the terrain */
void terrain_display (void);

/* terrain height at (x, y) */
float terrain_h (float x, float y);

/* free any memory associated with this module */
void terrain_free (void);

#endif /* TERRAIN_H */
