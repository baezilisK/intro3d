#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"
#include "cam.h"
#include "config.h"
#include "kbd.h"
#include "levelio.h"
#include "light.h"
#include "rain.h"
#include "rng.h"
#include "road.h"
#include "stars.h"
#include "terrain.h"
#include "texture.h"
#include "tree.h"
#include "util.h"

#define TOGGLE_FULLSCREEN 'f'
#define TOGGLE_RAINING 'k'
#define TOGGLE_DAYTIME 'j'

static int        /* [toggle] description */
  fullscreen = 0, /* [f] whether application is fullscreen */
  skipmouse = 0,  /* whether mouse handler should ignore next event */
  raining = 1;    /* [k] is it raining */
/*stars_daytime*/ /* [j] is it daytime */

static void init (void) {
  int u;
  glClearColor (0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  rng_init ();
  kbd_setmap ("assets/kbd/dvp");
  if (levelio_read ("assets/levels/12")) {
    fprintf (stderr, "fatal: specified level does not exist\n");
    exit (EXIT_FAILURE);
  }
  terrain_normalgen ();
  light_enable ();
  texture_load ();
  u = max (terrain_n, terrain_m); stars_init (CONFIG_NSTAR, 3*u, 10*u);
  glShadeModel (GL_SMOOTH);
  cam_ti = PI / 4;
}

static void main_exit (void) {
  tree_free ();
  terrain_free ();
  road_free ();
  light_disable ();
  texture_delete ();
  exit (EXIT_SUCCESS);
}

static void reshape (int w, int h) {
  glViewport (0, 0, w, h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (CONFIG_FOV, 1.0*w/h, CONFIG_NPLANE, CONFIG_FPLANE);
  cam_sync ();
}

static void passivemotion (int x, int y) {
  float s = 0.001, dx, dy;
  int
    cx = glutGet (GLUT_WINDOW_WIDTH) / 2,
    cy = glutGet (GLUT_WINDOW_HEIGHT) / 2;
  if (skipmouse) {
    skipmouse = 0;
  } else {
    dx = (cx - x) * s; dy = (cy - y) * s;
    cam_rotate (dx, dy);
    cam_sync ();
    glutPostRedisplay ();
  }
}

static void centermouse (void) {
  glutWarpPointer (
    glutGet (GLUT_WINDOW_WIDTH) / 2,
    glutGet (GLUT_WINDOW_HEIGHT) / 2
  );
}

static void keydown (unsigned char key, int x, int y) {
  (void) x; (void) y;
  if (kbd_map[key] == 'q') main_exit ();
  kbd_state[kbd_map[key]] = 1;
  if (kbd_map[key] == TOGGLE_FULLSCREEN) {
    fullscreen ^= 1;
    skipmouse = 1;
    if (fullscreen) glutFullScreen ();
    else glutReshapeWindow (CONFIG_WINDOW_H, CONFIG_WINDOW_H);
  } else if (kbd_map[key] == TOGGLE_RAINING) {
    raining ^= 1;
  } else if (kbd_map[key] == TOGGLE_DAYTIME) {
    stars_daytime ^= 1;
  }
}

static void keyup (unsigned char key, int x, int y) {
  (void) x; (void) y;
  kbd_state[kbd_map[key]] = 0;
}

static void trackfps (void) {
  static int now, then, t;
  now = glutGet (GLUT_ELAPSED_TIME);
  t = now - then;
  printf ("Frame delay:\t%d ms (%.1f FPS)\n", t, 1000.0 / t);
  then = glutGet (GLUT_ELAPSED_TIME);
}

static void axes (void) {
  glLineWidth (3);
  glBegin (GL_LINES);
    glColor3f (1, 0, 0); glVertex3f (1, 0, 0); glVertex3f (0, 0, 0);
    glColor3f (0, 1, 0); glVertex3f (0, 1, 0); glVertex3f (0, 0, 0);
    glColor3f (0, 0, 1); glVertex3f (0, 0, 1); glVertex3f (0, 0, 0);
  glEnd ();
  glLineWidth (1);
  glColor3f (1, 1, 1);
}

static void display (void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  axes ();
  light_sun ();
  terrain_display ();
  tree_displayall ();
  road_displayall ();
  if (raining) rain_display ();
  else stars_display ();
  centermouse ();
  glutSwapBuffers ();
}

static void tick (void) {
  float u = CONFIG_MOVE_SPEED, v = CONFIG_RAIN_RADIUS;
  if (kbd_state['w']) cam_mv (u, 0);
  if (kbd_state['s']) cam_mv (-u, 0);
  if (kbd_state['a']) cam_mv (0, -u);
  if (kbd_state['d']) cam_mv (0, u);
  if (kbd_state['c']) cam_z -= u;
  if (kbd_state[' ']) cam_z += u;
  if (raining) {
    rain_mk (CONFIG_RAIN_DENSITY, cam_x-v, cam_x+v, cam_y-v, cam_y+v);
    rain_tick ();
  }
  /* cam_z = terrain_h (cam_x, cam_y) + CONFIG_AVATAR_HEIGHT; */
}

static void timer (int s) {
  (void) s;
  tick ();
  trackfps ();
  glutPostRedisplay ();
  glutTimerFunc (CONFIG_FRAMEDELAY, timer, 0);
}

int main (int argc, char **argv) {
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow (argv[0]);
  init ();
  glutReshapeFunc (reshape);
  glutDisplayFunc (display);
  glutPassiveMotionFunc (passivemotion);
  glutKeyboardFunc (keydown);
  glutKeyboardUpFunc (keyup);
  glutTimerFunc (CONFIG_FRAMEDELAY, timer, 0);
  glutMainLoop ();
  exit (EXIT_FAILURE);
}
