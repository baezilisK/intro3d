#include <assert.h>
#include <stdlib.h>
#include "zutil.h"

void *zmalloc (size_t n) {
  void *ret = malloc (n);
  assert (ret);
  return ret;
}
