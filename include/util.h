#ifndef UTIL_H
#define UTIL_H

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define len(a) (sizeof(a) / sizeof(*(a)))

#define PI 3.1415926535

void *xmalloc (size_t);

void xfree (void *);

void crossproduct (float *u, float *v, float *uxv);

float dotproduct (float *u, float *v, int dim);

void normalize (float *u, int dim);

float lerp (float a, float b, float t);

float interpolate (float a, float b, float t);

void util_rprism (
    float x0, float x1, 
    float y0, float y1, 
    float z0, float z2);

#endif /* UTIL_H */
