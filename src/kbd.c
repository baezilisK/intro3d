#include <stdio.h>
#include "kbd.h"

static void loadid (void) {
  int i;
  for (i = 0; i < sizeof kbd_map / sizeof *kbd_map; ++i) {
    kbd_map[i] = i;
  }
}

int kbd_setmap (char *path) {
  FILE *f;
  int ret = 0, c, i;
  f = fopen (path, "r");
  if (f) {
    for (c = 0; c < sizeof kbd_map / sizeof *kbd_map; ++c)
      kbd_map[c] = c;
    i = 0;
    while (c = fgetc (f), c != EOF && i <= UCHAR_MAX) kbd_map[i++] = c;
  } else ret = 1;
  if (ret) loadid ();
  return ret;
}
