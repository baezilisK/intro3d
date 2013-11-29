#ifndef TEXTURE_H
#define TEXTURE_H

unsigned int texture_sun_name;
int texture_sun_dim;
unsigned char *texture_sun;

unsigned int texture_terrain_name;
int texture_terrain_dim;
unsigned char *texture_terrain;

unsigned int texture_tree_name;
int texture_tree_dim;
unsigned char *texture_tree;

unsigned int texture_leaf_name;
int texture_leaf_dim;
unsigned char *texture_leaf;

unsigned int texture_road_name;
int texture_road_dim;
unsigned char *texture_road;

unsigned int texture_cube_name;
int texture_cube_dim;
unsigned char *texture_cube;

void texture_enable (void *texture,
  unsigned int *name, int dim, int mode);

void texture_disable (unsigned int *name);

void texture_id (void);

void texture_checker (void);

unsigned char *texture_loadraw (char *path, int dim, int *cpdim);

unsigned char *texture_loadraw_hdr (  
  int hdr, char *path, int dim, int *cpdim);

void texture_useraw (void);

#endif /* TEXTURE_H */
