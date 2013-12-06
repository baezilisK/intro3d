#ifndef LIGHT_H
#define LIGHT_H

float 
  light_dayambient, light_daydiffuse, light_dayspecular,
  light_nightambient, light_nightdiffuse, light_nightspecular;

float light_sunx[4], light_suntheta;

void light_settime (int night);

void light_enable ();

void light_sun (void);

void light_disable (void);

#endif /* LIGHT_H */
