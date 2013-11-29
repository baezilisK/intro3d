#ifndef LIGHT_H
#define LIGHT_H

float light_sunx[3], light_suntheta;

/*
 * this calculates the angle between the sun's starting position and
 * the horizon. necessary for initialization of light_suntheta.
 */
float light_sunangle (void);

void light_enable (void);

void light_drawsun (void);

void light_drawtorch (void);

#endif /* LIGHT_H */
