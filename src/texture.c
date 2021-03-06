#include <assert.h>
#include <stdio.h>
#include "GL/gl.h"
#include "texture.h"
#include "util.h"

static char *names[] = {
  "assets/textures/wood.raw",
  "assets/textures/leaf.raw",
  "assets/textures/grass.raw",
  "assets/textures/road.raw",
  "assets/textures/sun.raw",
  "assets/textures/moon.raw"
};

static size_t length[] = {
  256,
  256,
  256,
  128,
  512,
  256
};

static unsigned *id;

static unsigned char **data;

static void loadraw (FILE *in, void *buf, size_t n) {
  char *a = buf;
  int i;
  for (i = 0; i < n; ++i) {
    a[i] = fgetc (in);
  }
}

void texture_enable (int texid) {
  assert (0 <= texid && texid < len (names));
  assert (len (names) == len (length));
  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, id[texid]);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D (
    GL_TEXTURE_2D, 0, GL_RGB, length[texid], length[texid], 0,
    GL_RGB, GL_UNSIGNED_BYTE, data[texid]
  );
}

void texture_disable (void) {
  glDisable (GL_TEXTURE_2D);
}

void texture_load (void) {
  int i;
  FILE *f;
  size_t size;
  id = xmalloc (len (names) * sizeof *id);
  data = xmalloc (len (names) * sizeof *data);
  for (i = 0; i < len (names); ++i) {
    size = 3 * length[i] * length[i];
    data[i] = xmalloc (size);
    f = fopen (names[i], "r"); assert (f);
    loadraw (f, data[i], size);
    fclose (f);
  }
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glGenTextures (len (names), id);
}

void texture_delete (void) {
  glDeleteTextures (len (names), id);
}
