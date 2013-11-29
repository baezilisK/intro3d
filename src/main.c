#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"
#include "sys/time.h"
#include "camera.h"
#include "config.h"
#include "kbd.h"
#include "levelio.h"
#include "light.h"
#include "rain.h"
#include "road.h"
#include "terrain.h"
#include "texture.h"
#include "tree.h"

#define KBD_ESC 0x1B
#define PI 3.14159265358979323846
#define W 640
#define H 480

static int
  ignorenextpassivemotion = 0,
  docenter = 1,
  rain = 0,
  pause = 0,
  noclip = 0,
  fullscreen = 0;

void tick (void) {
  if (pause) return;
  if (rain) {
    rain_mk ();
    rain_tick ();
  }
  if (config_daynight)
    light_suntheta = fmod (light_suntheta + 0.001, 2 * PI);
  if (kbd_state['w']) camera_move (CONFIG_MOVE_SPEED, 0);
  if (kbd_state['s']) camera_move (-CONFIG_MOVE_SPEED, 0);
  if (kbd_state['a']) camera_move (0, -CONFIG_MOVE_SPEED);
  if (kbd_state['d']) camera_move (0, CONFIG_MOVE_SPEED);
  if (noclip) {
    if (kbd_state[' ']) camera_z += CONFIG_MOVE_SPEED;
    if (kbd_state['c']) camera_z -= CONFIG_MOVE_SPEED;
  } else camera_z = terrain_h (camera_x, camera_y) + 1;
  camera_setmatrix ();
  glutPostRedisplay ();
}

static void centermouse (void) {
  if (docenter) glutWarpPointer (
    glutGet (GLUT_WINDOW_WIDTH) / 2,
    glutGet (GLUT_WINDOW_HEIGHT) / 2
  );
}

void randomize (void) {
  struct timeval tv;
  gettimeofday (&tv, NULL);
  srand (tv.tv_sec * tv.tv_usec);
}

void init(void) {
  randomize ();
  glClearColor(0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  glShadeModel (GL_SMOOTH);
  camera_setx (0, 0, 0);
  light_enable ();
  light_suntheta = light_sunangle ();
  terrain_getnormals ();
  texture_sun = texture_loadraw (
    "assets/textures/lava.raw",
    512, &texture_sun_dim
  );
  texture_terrain = texture_loadraw (
    "assets/textures/dirt.raw",
    256, &texture_terrain_dim
  );
  texture_tree = texture_loadraw (
    "assets/textures/wood2.raw",
    256, &texture_tree_dim
  );
  texture_leaf = texture_loadraw (
    "assets/textures/texture.raw",
    256, &texture_leaf_dim
  );
  texture_road = texture_loadraw (
    "assets/textures/road.raw",
    128, &texture_road_dim
  );
  texture_cube = texture_loadraw (
    "assets/textures/cube.raw",
    256, &texture_cube_dim
  );
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  centermouse ();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1.0*w/h, 0.01, 1000);
  camera_setmatrix ();
}

void axes (void) {
  glBegin (GL_LINES);
    glColor3f (1, 0, 0); glVertex3f (1, 0, 0); glVertex3f (0, 0, 0);
    glColor3f (0, 1, 0); glVertex3f (0, 1, 0); glVertex3f (0, 0, 0);
    glColor3f (0, 0, 1); glVertex3f (0, 0, 1); glVertex3f (0, 0, 0);
  glEnd ();
}

void avatar (void) {
  double dx[3]; camera_getdx (dx);
  glColor3f (1, 1, 1);
  glPushMatrix ();
    glTranslatef (camera_x + dx[0], camera_y + dx[1], camera_z + dx[2]);
    glScalef (0.33, 0.33, 0.33);
    glTranslatef (-0.5, -0.5, -0.5);
    texture_enable (texture_cube, &texture_cube_name,
      texture_cube_dim, GL_MODULATE);
    glBegin (GL_QUADS);
      glTexCoord2f (0, 0); glVertex3f (0, 0, 0);
      glTexCoord2f (0, 1); glVertex3f (0, 0, 1);
      glTexCoord2f (1, 1); glVertex3f (0, 1, 1);
      glTexCoord2f (1, 0); glVertex3f (0, 1, 0);

      glTexCoord2f (0, 0); glVertex3f (0, 0, 0);
      glTexCoord2f (0, 1); glVertex3f (0, 0, 1);
      glTexCoord2f (1, 1); glVertex3f (1, 0, 1);
      glTexCoord2f (1, 0); glVertex3f (1, 0, 0);

      glTexCoord2f (0, 0); glVertex3f (0, 0, 0);
      glTexCoord2f (0, 1); glVertex3f (0, 1, 0);
      glTexCoord2f (1, 1); glVertex3f (1, 1, 0);
      glTexCoord2f (1, 0); glVertex3f (1, 0, 0);

      glTexCoord2f (0, 0); glVertex3f (1, 0, 0);
      glTexCoord2f (0, 1); glVertex3f (1, 0, 1);
      glTexCoord2f (1, 1); glVertex3f (1, 1, 1);
      glTexCoord2f (1, 0); glVertex3f (1, 1, 0);

      glTexCoord2f (0, 0); glVertex3f (0, 1, 0);
      glTexCoord2f (0, 1); glVertex3f (0, 1, 1);
      glTexCoord2f (1, 1); glVertex3f (1, 1, 1);
      glTexCoord2f (1, 0); glVertex3f (1, 1, 0);

      glTexCoord2f (0, 0); glVertex3f (0, 0, 1);
      glTexCoord2f (0, 1); glVertex3f (0, 1, 1);
      glTexCoord2f (1, 1); glVertex3f (1, 1, 1);
      glTexCoord2f (1, 0); glVertex3f (1, 0, 1);
    glEnd ();
    texture_disable (&texture_cube_name);
  glPopMatrix ();
}

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  light_enable ();
  light_drawsun ();
  light_drawtorch ();
  centermouse ();
  terrain_display ();
  tree_displayall ();
  road_displayall ();
  if (rain) rain_display ();
  /* axes (); */
  if (config_thirdperson) avatar ();
  glFlush();
}

void passivemotion (int x, int y) {
  float rotatescale = 0.001, dx, dy;
  int
    cx = glutGet (GLUT_WINDOW_WIDTH) / 2,
    cy = glutGet(GLUT_WINDOW_HEIGHT) / 2;
  if (ignorenextpassivemotion) {
    ignorenextpassivemotion = 0;
    return;
  }
  dx = (cx - x) * rotatescale; dy = (cy - y) * rotatescale;
  if (dx || dy) { 
    camera_rotate (dx, dy);
    camera_setmatrix ();
    glutPostRedisplay ();
  }
}

void motion (int x, int y) {
  passivemotion (x, y);
}

void keydown (unsigned char key, int x, int y) {
  (void) x; (void) y;
  kbd_state[kbd_map[key]] = 1;
  if (kbd_map[key] == KBD_ESC) docenter ^= 1;
  if (kbd_map[key] == 'q') exit (EXIT_SUCCESS);
  if (kbd_map[key] == 'p') pause ^= 1;
  if (kbd_map[key] == 'o') config_thirdperson ^= 1;
  if (kbd_map[key] == 'i') config_daynight ^= 1;
  if (kbd_map[key] == 'l') noclip ^= 1;
  if (kbd_map[key] == 'k') rain ^= 1;
  if (kbd_map[key] == 'f') {
    fullscreen ^= 1;
    ignorenextpassivemotion = 1;
    if (fullscreen) glutFullScreen ();
    else glutReshapeWindow (W, H);
  }
}

void keyup (unsigned char key, int x, int y) {
  (void) x; (void) y;
  kbd_state[kbd_map[key]] = 0;
}

void specialfunc (int key, int x, int y) {
  (void) x; (void) y;
  if (key == GLUT_KEY_UP) camera_move (0.5, 0);
  if (key == GLUT_KEY_DOWN) camera_move (-0.5, 0);
  if (key == GLUT_KEY_LEFT) camera_roti += 0.01;
  if (key == GLUT_KEY_RIGHT) camera_roti -= 0.01;
}

int main (int argc, char **argv) {
  if (argc == 2) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(W, H);
    glutCreateWindow (argv[0]);
    levelio_read (argv[1]); assert (!levelio_errno);
    kbd_setmap ("assets/kbd/dvp");
    init();
    glutIgnoreKeyRepeat (4);
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutKeyboardFunc (keydown);
    glutKeyboardUpFunc (keyup);
    glutPassiveMotionFunc (passivemotion);
    glutSpecialFunc (specialfunc);
    glutIdleFunc (tick);
    glutMainLoop();
  } else {
    fprintf (stderr, "usage: %s <levelfile>\n", argv[0]);
  }
  exit (EXIT_FAILURE);
}
