#ifndef STARS_H
#define STARS_H

#define STARS_MAX 1000

int stars_daytime;

/* create n (n < STARS_MAX) random stars with r <= radius <= R */
void stars_init (int n, float r, float R);

/* indicate to the module that a unit of time has passed */
void stars_tick (void);

/* render the stars */
void stars_display (void);

#endif /* STARS_H */
