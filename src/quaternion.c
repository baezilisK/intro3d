#include <math.h>
#include <string.h>
#include "quaternion.h"

static double dotproduct (double *u, double *v, int n) {
  int i;
  double ret = 0;
  for (i = 0; i < n; ++i) {
    ret += u[i] * v[i];
  }
  return ret;
}

static void crossproduct (double *u, double *v) {
  double r[3];
    r[0] = u[1]*v[2] - u[2]*v[1];
    r[1] = u[2]*v[0] - u[0]*v[2];
    r[2] = u[0]*v[1] - u[1]*v[0];
  memcpy (u, r, sizeof r);
}

void quaternion_mk (H r, double re, double i, double j, double k) {
  r[0] = re; r[1] = i; r[2] = j; r[3] = k;
}

void quaternion_mul (H p, H q) {
  int i;
  H r; memcpy (r, p, sizeof r);

  r[0] = *p * *q - dotproduct (p + 1, q + 1, QUATERNION_DIM - 1);
  crossproduct (r + 1, q + 1);
  for (i = 1; i < QUATERNION_DIM; ++i) {
    r[i] += *p * q[i];
    r[i] += *q * p[i];
  }

  memcpy (p, r, sizeof r);
}

void quaternion_add (H p, H q) {
  int i;
  for (i = 0; i < QUATERNION_DIM; ++i) {
    p[i] += q[i];
  }
}

void quaternion_exp (H r, double t) {
  int i;
  r[0] = cos (t);
  for (i = 1; i < QUATERNION_DIM; ++i) r[i] *= sin (t);
}

void normalize (H r) {
  int i;
  double norm = 0;
  for (i = 0; i < QUATERNION_DIM; ++i) norm += r[i]*r[i];
  norm = sqrt (norm);
  for (i = 0; i < QUATERNION_DIM; ++i) r[i] /= norm;
}
