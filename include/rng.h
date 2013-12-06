/* 
 * utility for random numbers
 */

#ifndef RNG_H
#define RNG_H

/* must be called before anything else in this header */
void rng_init (void);

/* returns an integer in [0, n) */
int rng_i (int n);

/* returns a real in [0, 1] */
double rng_f (void);

#endif /* RNG_H */
