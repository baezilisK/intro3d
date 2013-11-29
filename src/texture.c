#include <math.h>
#include <stdio.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "texture.h"
#include "zutil.h"

void texture_enable (void *texture,
    unsigned int *name, int dim, int mode) {
  glEnable (GL_TEXTURE_2D);
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glGenTextures (1, name);
  glBindTexture (GL_TEXTURE_2D, *name);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D (
    GL_TEXTURE_2D, 0, GL_RGBA, dim, dim, 0,
    GL_RGB, GL_UNSIGNED_BYTE, texture
  );
}

void texture_disable (unsigned int *name) {
  glDeleteTextures (1, name);
  glDisable (GL_TEXTURE_2D);
}

unsigned char *texture_loadraw (char *fname, int dim, int *d) {
  FILE *f = fopen (fname, "rb");
  size_t n = 3 * dim * dim;
  unsigned char *ret = zmalloc (n * sizeof *ret);
  int i = 0, c; (void) c;
  *d = dim;
  
  if (f) {
    while (c = fgetc (f), c != EOF && i < n) {
      ret[i++] = c;
    }
    fclose (f);
    gluBuild2DMipmaps (
      GL_TEXTURE_2D, GL_RGB, dim, dim, GL_RGB,
      GL_UNSIGNED_BYTE, ret
    );
  }
  return ret;
}
