#ifndef CAMERA_H
#define CAMERA_H

double 
  camera_x, camera_y, camera_z,
  camera_roti, camera_rotj;

void camera_setx (double x, double y, double z);

void camera_getdx (double *u);

/*
 * move forward/back/left/right by the given amount, eg
 * move(3, -0.1): forward, and left a bit
 */
void camera_move (double x, double y);

void camera_rotate (double a, double b);

/*
 * synchronise GL modelview matrix with the camera
 */
void camera_setmatrix (void);

#endif /* CAMERA_H */
