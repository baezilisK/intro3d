#include "GL/gl.h"
#include "light.h"

void light_enable (void) {
  float
    amb[] = {0.1, 0.1, 0.1, 1},
    spe[] = {0.6, 0.6, 0.6, 1},
    dif[] = {0.4, 0.4, 0.4, 1};
  glLightfv (GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, spe);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, amb);
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
