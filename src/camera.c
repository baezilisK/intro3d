#include <math.h>
#include "camera.h"
#include "GL/glu.h"

#define PI 3.14159265359

void camera_setx (double x, double y, double z) {
  camera_x = x; camera_y = y; camera_z = z;
}

void camera_move (double dx, double dy) {
  camera_x += dx * cos (camera_roti);
  camera_y += dx * sin (camera_roti);
  camera_x += dy * cos (camera_roti - PI/2);
  camera_y += dy * sin (camera_roti - PI/2);
}

void camera_rotate (double a, double b) {
  camera_roti += a;
  if (camera_roti > PI) camera_roti -= 2*PI; 
  if (camera_roti < -PI) camera_roti += 2*PI;

  if (fabs (camera_rotj + b) < PI / 2) camera_rotj += b;
}

void camera_getdx (double *dx) {
  dx[0] = cos (camera_roti) * cos (camera_rotj);
  dx[1] = sin (camera_roti) * cos (camera_rotj);
  dx[2] = sin (camera_rotj);
}

void camera_setmatrix (void) {
  double x = camera_x, y = camera_y, z = camera_z, dx[3];
  camera_getdx (dx);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (
    x, y, z,
    x + dx[0], y + dx[1], z + dx[2],
    0, 0, 1
  );
}
