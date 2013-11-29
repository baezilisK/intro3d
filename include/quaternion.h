#ifndef QUATERNION_H
#define QUATERNION_H

#define QUATERNION_DIM 4

typedef double quaternion[QUATERNION_DIM];
typedef quaternion H;

void quaternion_mk (H, double re, double i, double j, double k);
void quaternion_mul (H, H);
void quaternion_add (H, H);
void quaternion_exp (H r, double t); /* r := e ^ (rt) */
void quaternion_normalize (H);

#endif /* QUATERNION_H */
