#ifndef UTIL_H
#define UTIL_H

void *xmalloc (size_t);

void xfree (void *);

float lerp (float a, float b, float t);

float interpolate (float a, float b, float t);

#endif /* UTIL_H */
