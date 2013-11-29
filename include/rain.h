#ifndef RAIN_HDR
#define RAIN_HDR

#define RAIN_N 10000
#define RAIN_H 10
#define RAIN_V 0.03

int rain_n, rain_i, rain_j;
float rain_x[RAIN_N], rain_y[RAIN_N], rain_z[RAIN_N];

void rain_mk (void);

void rain_tick (void);

void rain_display (void);

#endif
