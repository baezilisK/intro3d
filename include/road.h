#ifndef ROAD_H
#define ROAD_H

int road_n;

struct road {
  float w;
  int nctrl;
  float *sx, *sy;
};

struct road *road_all;

void road_displayall (void);

#endif /* ROAD_H */
