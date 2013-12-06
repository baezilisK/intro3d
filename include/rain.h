#ifndef RAIN_H
#define RAIN_H

#define RAIN_MAX 100000

/* create k drops of rain, up to a maximum of RAIN_MAX */
void rain_mk (int k, float x0, float x1, float y0, float y1);

/* indicate to the module that a unit of time has passed */
void rain_tick (void);

/* display the rain */
void rain_display (void);

#endif /* RAIN_H */
