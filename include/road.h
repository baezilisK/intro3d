#ifndef ROAD_H
#define ROAD_H

int road_n;

struct road {
  float w;

  int nctrl;
  float *cx, *cy;
};

struct road *road_all;

void road_displayall (void);

void road_free (void);

#endif /* ROAD_H */
