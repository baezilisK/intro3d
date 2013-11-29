#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define len(a) (sizeof (a) / sizeof *(a))

void *zrealloc (void *p, size_t n) {
  void *ret = realloc (p, n);
  assert (ret);
  return ret;
}

void *zmalloc (size_t n) {
  return zrealloc (NULL, n);
}

void usage (void) {
  fprintf (stderr, "usage: ./levelgen c\n");
  fprintf (stderr, "reads json from stdin, writes level to stdout\n");
}

void json (char *s) {
  float sun[3];
  int terrain_n = 0, terrain_m = 0, *terrain_h;
  int tree_n = 0;
  float *tree_x = NULL, *tree_y = NULL;
  int road_n = 0, *road_nspine = {0};
  float *road_w = NULL, **road_spine = NULL;

  int i, j, st = 0;
  char *t, *var, *fmt, *u;

  var = "\"sunlight\":"; fmt = "[%f,%f,%f]";
  if (t = strstr (s, var), t)
    sscanf (t + strlen (var), fmt, sun, sun+1, sun+2);

  var = "\"width\":"; fmt = "%d";
  for (i = 0; s[i]; ++i) {
    if (strchr ("([{", s[i])) ++st;
    else if (strchr ("}])", s[i])) --st;
    if (st == 0 && strstr (s + i, var)) {
      sscanf (s + i + strlen (var), fmt, &terrain_n);
      break;
    }
  }

  var = "\"depth\":"; fmt = "%d";
  if (t = strstr (s, var), t)
    sscanf (t + strlen (var), fmt, &terrain_m);

  terrain_h = zmalloc (terrain_n*terrain_m * sizeof *terrain_h);
  var = "\"altitude\":";
  if (t = strstr (s, var), t) {
    for (; *t != '['; ++t); ++t;
    for (i = 0; i < terrain_n * terrain_m; ++i) {
      sscanf (t, "%d,", &terrain_h[i]);
      for (; *t && *t != ','; ++t); ++t;
    }
  }

  var = "\"trees\":";
  if (t = strstr (s, var), t) {
    int n = 0, m = 8;
    tree_x = zmalloc (m * sizeof *tree_x);
    tree_y = zmalloc (m * sizeof *tree_y);
    for (; *t != ']'; ++t) {
      if (n == m) {
        m *= 2;
        tree_x = zrealloc (tree_x, m * sizeof *tree_x);
        tree_y = zrealloc (tree_y, m * sizeof *tree_y);
      }

      var = "\"x\":";
      u = strstr (t, var); assert (u);
      sscanf (u + strlen (var), "%f", &tree_x[n]);

      var = "\"z\":";
      u = strstr (t, var); assert (u);
      sscanf (u + strlen (var), "%f", &tree_y[n]);

      ++n;
      for (; *t != '}'; ++t); ++t;
    }

    tree_n = n;
  }

  var = "\"roads\":";
  if (t = strstr (s, var), t) {
    int n = 0, m = 8;
    road_nspine = zmalloc (m * sizeof *road_nspine);
    road_spine = zmalloc (m * sizeof *road_spine);
    road_w = zmalloc (m * sizeof *road_w);
    for (; *t && *t != ']'; ++t) {
      if (n == m) {
        m *= 2;
        road_nspine = zrealloc (road_nspine, m * sizeof *road_nspine);
        road_w = zrealloc (road_w, m * sizeof *road_w);
        road_spine = zrealloc (road_spine, m * sizeof *road_spine);
      }

      var = "\"width\":";
      u = strstr (t, var); assert (u);
      sscanf (u + strlen (var), "%f", &road_w[n]);

      var = "\"spine\":";
      u = strstr (t, var); assert (u);
      {
        int nn = 0, mm = 8;
        road_spine[n] = zmalloc (mm * sizeof *road_spine[n]);
        for (u += 1 + strlen (var); *u != '}'; ++u) {
          if (nn == mm) {
            mm *= 2;
            road_spine[n] = zrealloc (road_spine[n],
              mm * sizeof *road_spine[n]
            );
          }
          sscanf (u, "%f", &road_spine[n][nn]);
          for (; *u != ',' && *u != ']'; ++u);
          ++nn;
        }

        road_nspine[n] = nn;
      }

      ++n;
      for (; *t != '}'; ++t); ++t;
    }

    road_n = n;
  }

  (void) tree_n;

  printf ("%f %f %f\n", sun[0], sun[2], sun[1]);
  printf ("%d %d\n", terrain_n, terrain_m);
  for (i = 0; i < terrain_n * terrain_m; ++i)
    printf ("%d ", terrain_h[i]);
  printf ("\n");
  printf ("%d\n", tree_n);
  for (i = 0; i < tree_n; ++i) printf ("%f %f\n", tree_x[i], tree_y[i]);
  printf ("%d\n", road_n);
  for (i = 0; i < road_n; ++i) {
    printf ("%d %f ", road_nspine[i]/2, road_w[i]);
    for (j = 0; j < road_nspine[i]; ++j)
      printf ("%f ", road_spine[i][j]);
    printf ("\n");
  }
  free (terrain_h);
  free (tree_x); free (tree_y);
  for (i = 0; i < road_n; ++i) free (road_spine[i]);
  free (road_nspine); free (road_w);
}

int main (int argc, char **argv) {
  int ret = EXIT_SUCCESS;
  (void) argv;
  if (argc != 2) {
    usage ();
    ret = EXIT_FAILURE;
  } else {
    int i = 0, n = 8, c;
    char *buf = zmalloc (n);
    while (c = getchar (), c != EOF) {
      if (!isspace (c)) {
        if (i == n) {
          n *= 2;
          buf = zrealloc (buf, n);
        }
        if (i > 0 || c != '{') buf[i++] = c;
      }
    }
    if (i > 0 && buf[i - 1] == '}') --i;
    else if (i == n) buf = zrealloc (buf, n + 1);
    buf[i] = '\0';
    json (buf);
    free (buf);
  }
  exit (ret);
}
