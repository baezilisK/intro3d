#ifndef TEXTURE_H
#define TEXTURE_H

enum {
  TEXTURE_TREE,
  TEXTURE_GRASS,
  TEXTURE_ROAD,
  TEXTURE_SUN
};

/* load textures into memory */
void texture_load (void);

/* enable specified texture */
void texture_enable (int texid);

/* disable specified texture */
void texture_disable (void);

/* delete texture memory */
void texture_delete (void);

#endif /* TEXTURE_H */
