#ifndef STARS_H
#define STARS_H

#define STARS_MAX 1000

/* create n (n < STARS_MAX) random stars with r <= radius <= R */
void stars_init (int n, float r, float R);

/* render the stars */
void stars_display (void);

#endif /* STARS_H */
