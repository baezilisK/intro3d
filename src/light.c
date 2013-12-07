#include "GL/gl.h"
#include "light.h"
#include "stars.h"
#include "util.h"

void light_settime (int night) {
  int i;
  float amb[4], spe[4], dif[4];
  light_dayambient = 0.16; light_nightambient = 0.08;
  light_daydiffuse = 0.6; light_nightdiffuse = 0.2;
  light_dayspecular = 0.8; light_nightspecular = 0.3;
  for (i = 0; i < len (amb); ++i) {
    amb[i] = night ? light_nightambient : light_dayambient;
    spe[i] = night ? light_nightspecular : light_dayspecular;
    dif[i] = night ? light_nightdiffuse : light_daydiffuse;
  }
  if (night) glClearColor (0, 0, 0, 1);
  else glClearColor (0.1, 0.3, 0.8, 1);
  glLightfv (GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, spe);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, amb);
}

void light_enable (void) {
  light_settime (0);
  glEnable (GL_LIGHTING);
  glEnable (GL_COLOR_MATERIAL);
  glEnable (GL_LIGHT0);
}

void light_sun (void) {
  glLightfv (GL_LIGHT0, GL_POSITION, light_sunx);
}

void light_disable (void) {
  glDisable (GL_LIGHT0);
  glDisable (GL_LIGHTING);
}
