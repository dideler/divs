#include <iostream>
#include <cstdlib>
#include <cstdio> // for flushing stdout
#include <cmath>
#include <GL/glut.h>
#include <GL/glx.h> // for the fonts
#include <ctime>
#include <cstring> // for strlen
#include "graphics.h"
#include "board.h"

using namespace std;

GLboolean graphics::lightSwitch, graphics::mouseRotate, graphics::texBlend,
graphics::textures, graphics::editMode, graphics::deleteMode,
graphics::addMode, graphics::marble, graphics::twoPlayer;
GLuint graphics::fps, graphics::width, graphics::height, graphics::frameDelay,
graphics::plyLevel, graphics::base;
GLint graphics::mX, graphics::mY, graphics::yRot, graphics::xRot;
GLfloat graphics::lightPosition[4];

graphics::drawType graphics::drawMode;
graphics::menuOptions graphics::addUnit;
board * graphics::brd;

graphics* graphics::glClass;

graphics::graphics (int argc, char** argv)
{
  srand (time (NULL));
  glutInit (&argc, argv);
  glClass = this;
  init ();
}

void
graphics::init ()
{
  plyLevel = 3;
  yRot = xRot = 0;
  drawMode = SOLID;
  mouseRotate = editMode = deleteMode = addMode = marble = twoPlayer = GL_FALSE;
  lightSwitch = texBlend = textures = GL_TRUE;
  height = 600;
  width = 800;
  fps = 50; // should be dividable by 1000 (i.e. 1 second = 1000 ms) */
  /* glut timer only takes an int delay, so choose an fps that gives you
   * a whole number when 1000 is divided by it */
  frameDelay = 1000 / fps;

  cout << "Default depth is " << plyLevel << " ply.\n\n"
          "==CONTROLS==\nQ or ESC:\tQuit\nLeft arrow:\tRotate board (y-axis)\nRight arrow:"
          "\tRotate board (y-axis)\nUp arrow:\tRotate board(x-axis)\nDown arrow:"
          "\tRotate board(x-axis)\nRight click:\tControl menu\nLeft click:\tSelection\n"
          "Middle click:\tRotate while held down (x- & y-axis)\n" << endl;

  glutInitWindowSize (width, height);
  glutInitWindowPosition (250, 250);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("Deep Blue Sea");

  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard1);
  glutSpecialFunc (keyboard2);
  glutMouseFunc (mouse);
  glutMotionFunc (motion);
  initMenu ();
  buildFont ();

  glClearColor (0.0, 0.0, 0.0, 1.0); // set to black
  glEnable (GL_DEPTH_TEST);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // nice perspective calculations
  glHint (GL_POINT_SMOOTH_HINT, GL_NICEST); // nice point smoothing

  //create board and chess pieces here so they can load textures
  brd = new board (board::BLACK); //AI is set to BLACK by default

  glEnable (GL_LIGHTING);
  glShadeModel (GL_SMOOTH);

  graphics::lightPosition[0] = 44.0;
  graphics::lightPosition[1] = 20.0;
  graphics::lightPosition[2] = 22.0;
  graphics::lightPosition[3] = 1.0;
  GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);

  glutTimerFunc (100, animator, frameDelay);
  glutMainLoop ();
}

/* based off a NeHe tutorial (from gamedev.net) */
void
graphics::buildFont ()
{
  Display * dpy;
  XFontStruct * fontInfo; // storage for the font

  base = glGenLists (96); // storage for 96 characters

  // Load the font
  // This opens a second connection to the display in the DISPLAY environment
  // value, and will be around only long enough to load the font.
  dpy = XOpenDisplay (NULL); // default to DISPLAY env

  fontInfo = XLoadQueryFont (dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1");
  if (fontInfo == NULL)
    {
      fontInfo = XLoadQueryFont (dpy, "fixed");
      if (fontInfo == NULL) cout << "no X font available?" << endl;
    }

  // after loading this font info, this would probably be the time
  // to rotate, scale, or otherwise twink your fonts.

  // start at character 32 (space), get 96 characters (a few characters past z), and
  // store them starting at base.
  glXUseXFont (fontInfo->fid, 32, 96, base);

  // free that font's info now that we've got the display lists.
  XFreeFont (dpy, fontInfo);

  // close down the 2nd display connection.
  XCloseDisplay (dpy);
}

void
graphics::killFont ()
{
  glDeleteLists (base, 96); // delete all 96 characters
}

void
graphics::glPrint (char * text) // custom gl print routine
{
  if (text == NULL) return; // no text, do nothing

  glPushAttrib (GL_LIST_BIT); // alert that we're about to offset the display lists with glListBase
  glListBase (base - 32); // sets the base character to 32

  glCallLists (strlen (text), GL_UNSIGNED_BYTE, text); // draws the display list text
  glPopAttrib (); // undoes the glPushAttrib(GL_LIST_BIT);
}

void
graphics::writeMsgs () //FIXME it only prints the message if stdout is flushed? (e.g. check)
{
  glRasterPos2f (18, 10); // set to a nice location
  switch (board::state)
    {
    case board::ONGOING :
      return;
    case board::WIN :
      glPrint ((char *) "Victory!");
      break;
    case board::LOSE :
      glPrint ((char *) "Defeat...");
      break;
    case board::DRAW :
      glPrint ((char *) "Stalemate");
      break;
    case board::EDIT :
      glPrint ((char *) "Edit Mode");
      break;
    case board::BLACK_IN_CHECK :
      glPrint ((char *) "Black is in Check!");
      //fflush (stdout);
      break;
    case board::WHITE_IN_CHECK :
      glPrint ((char *) "White is in Check!");
      break;
    default:
      return;
    }
}

/* Animator callback function:
 * Registers a timer callback to be triggered in a specified number of
 * milliseconds. */
void
graphics::animator (int delay)
{
  glutPostRedisplay ();
  glutTimerFunc (delay, animator, glClass->frameDelay);
}

/* Display callback function */
void
graphics::display ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  //rotate system coordinates
  glPushMatrix ();
  rotateBoard ();

  /* drawing code goes here */
  glClass->brd->drawBoard (GL_RENDER);
  //glCallList(glClass->brd->boardRender);
  writeMsgs ();

  glutSwapBuffers ();

  GLenum isError = glGetError ();
  if (isError != GL_NO_ERROR) /**< in case of gl error, display error */
    cerr << gluErrorString (isError) << endl;

  glPopMatrix ();
}

void
graphics::rotateBoard ()
{
  if (graphics::yRot != 0 || graphics::xRot != 0)
    {
      glTranslatef (22, 0, 22);
      glRotatef (graphics::xRot * 0.5, 1, 0, 0);
      glRotatef (graphics::yRot * 0.5, 0, 1, 0);
      glTranslatef (-22, 0, -22);
      //graphics::yRot %= 360;
      //graphics::xRot %= 360;
    }
}

/* Window reshape callback function -- called initially and on window reshape */
void
graphics::reshape (int w, int h)
{
  glClass->width = w;
  glClass->height = h;
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (40.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (20.5, 45.0, 80.0, // eye position
             20.5, 0.0, 20.5, // reference viewing point (0,0,0 = centre)
             0.0, 1.0, 0.0); // up vector
}

/* 3 steps to a menu: (1) decide menu entries (done in menu function above),
 * (2) decide text to display, (3) decide what mouse button to use */
void
graphics::initMenu ()
{
  int lightMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("Toggle light", M_LIGHT);
  glutAddMenuEntry ("Positional light", M_POSITIONAL);
  glutAddMenuEntry ("Directional light", M_DIRECTIONAL);

  int subMenuGraphics = glutCreateMenu (menu);
  glutAddSubMenu ("Lighting", lightMenu);
  glutAddMenuEntry ("Toggle textures", M_TEXTURE);
  glutAddMenuEntry ("Toggle chess pieces", M_TOGGLE_PIECES);
  glutAddMenuEntry ("Toggle texture blend", M_TEX_BLEND);
  //glutAddMenuEntry ("Toggle shadows", M_SHADOWS);
  //glutAddMenuEntry ("Toggle reflection", M_REFLECTION);

  int plyMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("1-ply", M_PLY_1);
  glutAddMenuEntry ("2-ply", M_PLY_2);
  glutAddMenuEntry ("3-ply", M_PLY_3);
  glutAddMenuEntry ("4-ply", M_PLY_4);
  glutAddMenuEntry ("5-ply", M_PLY_5);
  glutAddMenuEntry ("6-ply", M_PLY_6);
  //glutAddMenuEntry ("7-ply", M_PLY_7);

  int promoteMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("Queen", M_P_QUEEN);
  glutAddMenuEntry ("Rook", M_P_ROOK);
  glutAddMenuEntry ("Bishop", M_P_BISHOP);
  glutAddMenuEntry ("Knight", M_P_KNIGHT);

  int aiColourMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("Black", M_AI_BLACK);
  glutAddMenuEntry ("White", M_AI_WHITE);

  int opponentMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("Human", M_MULTIPLAYER);
  glutAddMenuEntry ("Computer", M_SOLO);

  int subMenuAI = glutCreateMenu (menu);
  glutAddSubMenu ("Ply Level", plyMenu);
  glutAddSubMenu ("Promotion Type", promoteMenu);
  glutAddSubMenu ("Opponent", opponentMenu);
  glutAddSubMenu ("AI plays as", aiColourMenu);

  int whitePieceMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("Pawn", M_W_PAWN);
  glutAddMenuEntry ("Bishop", M_W_BISHOP);
  glutAddMenuEntry ("Knight", M_W_KNIGHT);
  glutAddMenuEntry ("Rook", M_W_ROOK);
  glutAddMenuEntry ("Queen", M_W_QUEEN);
  glutAddMenuEntry ("King", M_W_KING);

  int blackPieceMenu = glutCreateMenu (menu);
  glutAddMenuEntry ("Pawn", M_B_PAWN);
  glutAddMenuEntry ("Bishop", M_B_BISHOP);
  glutAddMenuEntry ("Knight", M_B_KNIGHT);
  glutAddMenuEntry ("Rook", M_B_ROOK);
  glutAddMenuEntry ("Queen", M_B_QUEEN);
  glutAddMenuEntry ("King", M_B_KING);

  int colourMenu = glutCreateMenu (menu);
  glutAddSubMenu ("White", whitePieceMenu);
  glutAddSubMenu ("Black", blackPieceMenu);

  int subMenuEdit = glutCreateMenu (menu);
  glutAddMenuEntry ("Toggle edit mode", M_EDIT);
  glutAddSubMenu ("Add", colourMenu);
  glutAddMenuEntry ("Delete", M_DELETE);
  glutAddMenuEntry ("Clear board", M_CLEAR);

  int mainMenu = glutCreateMenu (menu); //NOTE ignore warning; not an unused variable!
  glutAddSubMenu ("Graphics", subMenuGraphics);
  glutAddSubMenu ("AI", subMenuAI);
  glutAddSubMenu ("Edit", subMenuEdit);
  glutAddMenuEntry ("Resign", M_RESIGN);
  glutAddMenuEntry ("Reset", M_RESET);
  glutAddMenuEntry ("Quit", M_QUIT);

  glutAttachMenu (GLUT_RIGHT_BUTTON);
}

/* Menu callback function */
void
graphics::menu (int value)
{
  switch (value)
    {
    case M_NONE:
      return;
    case M_RESET: // reset to initial configuration -- including graphics
      yRot = xRot = 0; // reset camera position
      for (int i = 0; i < 64; ++i) // clear board
        board::deletePiece (i);
      board::placePieces (); // place pieces on their original positions
      plyLevel = 3; // reset ply level to its default
      board::state = board::ONGOING; // reset board state
      editMode = deleteMode = addMode = GL_FALSE;
      lightSwitch = texBlend = textures = GL_TRUE;
      break;
    case M_RESIGN:
      board::state = board::LOSE;
      break;
    case M_QUIT:
      glClass->killFont ();
      exit (0);
      break;
    case M_MULTIPLAYER:
      twoPlayer = true;
      break;
    case M_SOLO:
      twoPlayer = false;
      board::aiSide = board::BLACK; // reset AI side
      yRot = xRot = 0; // reset camera position
      break;
    case M_LIGHT:
      lightSwitch = !lightSwitch;
      lightSwitch ? glEnable (GL_LIGHTING) : glDisable (GL_LIGHTING);
      break;
    case M_TEXTURE:
      textures = !textures;
      break;
    case M_TOGGLE_PIECES:
      marble = !marble;
      break;
    case M_TEX_BLEND:
      texBlend = !texBlend;
      if (texBlend)
        {
          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
          lightSwitch = false;
          glDisable (GL_LIGHTING);
        }
      else
        {
          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        }
      break;
      //case M_SHADOWS:
      //  break;
      //case M_REFLECTION:
      //  break;
    case M_POSITIONAL:
      graphics::lightPosition[3] = 1.0;
      break;
    case M_DIRECTIONAL:
      graphics::lightPosition[3] = 0.0;
      break;
    case M_PLY_1:
      plyLevel = 1;
      break;
    case M_PLY_2:
      plyLevel = 2;
      break;
    case M_PLY_3:
      plyLevel = 3;
      break;
    case M_PLY_4:
      plyLevel = 4;
      break;
    case M_PLY_5:
      plyLevel = 5;
      break;
    case M_PLY_6:
      plyLevel = 6;
      break;
      //case M_PLY_7:
      //  plyLevel = 7;
      //  break;
    case M_P_QUEEN:
      board::promoteTo = board::QUEEN;
      break;
    case M_P_ROOK:
      board::promoteTo = board::ROOK;
      break;
    case M_P_BISHOP:
      board::promoteTo = board::BISHOP;
      break;
    case M_P_KNIGHT:
      board::promoteTo = board::KNIGHT;
      break;
    case M_W_PAWN:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_W_PAWN;
        }
      break;
    case M_W_BISHOP:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_W_BISHOP;
        }
      break;
    case M_W_KNIGHT:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_W_KNIGHT;
        }
      break;
    case M_W_ROOK:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_W_ROOK;
        }
      break;
    case M_W_QUEEN:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_W_QUEEN;
        }
      break;
    case M_W_KING:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_W_KING;
        }
      break;
    case M_B_PAWN:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_B_PAWN;
        }
      break;
    case M_B_BISHOP:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_B_BISHOP;
        }
      break;
    case M_B_KNIGHT:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_B_KNIGHT;
        }
      break;
    case M_B_ROOK:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_B_ROOK;
        }
      break;
    case M_B_QUEEN:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_B_QUEEN;
        }
      break;
    case M_B_KING:
      if (editMode)
        {
          deleteMode = false;
          addMode = true;
          addUnit = M_B_KING;
        }
      break;
    case M_EDIT:
      editMode = !editMode;
      if (editMode) board::state = board::EDIT;
      else
        {
          board::state = board::ONGOING;
          board::squaresAttacked (board::WHITE, board::pieces); // update attacks
          board::squaresAttacked (board::BLACK, board::pieces);
          deleteMode = false;
          addMode = false;
        }
      break;
    case M_DELETE:
      if (editMode)
        {
          deleteMode = true;
          addMode = false;
        }
      break;
    case M_CLEAR:
      if (editMode)
        {
          for (int i = 0; i < 64; ++i)
            board::deletePiece (i);
        }
      break;
    case M_AI_BLACK:
      board::aiSide = board::BLACK;
      yRot = xRot = 0; // reset camera position
      for (int i = 0; i < 64; ++i) // clear board
        board::deletePiece (i);
      board::placePieces (); // place pieces on their original positions
      plyLevel = 3; // reset ply level to its default
      board::state = board::ONGOING; // reset board state
      editMode = deleteMode = addMode = GL_FALSE;
      lightSwitch = texBlend = textures = GL_TRUE;
      break;
    case M_AI_WHITE:
      board::aiSide = board::WHITE;
      yRot = 360;
      xRot = 0; // reset camera position
      for (int i = 0; i < 64; ++i) // clear board
        board::deletePiece (i);
      board::placePieces (); // place pieces on their original positions
      plyLevel = 3; // reset ply level to its default
      board::state = board::ONGOING; // reset board state
      editMode = deleteMode = addMode = GL_FALSE;
      lightSwitch = texBlend = textures = GL_TRUE;
      brd->makeAiMove ();
      break;
    }
}

/* Keyboard input callback function */
void
graphics::keyboard1 (unsigned char key, int x, int y)
{
  switch (key)
    {
    case 27:
    case 'q':
    case 'Q':
      glClass->killFont ();
      exit (0);
      break;
    default:
      cout << "Key '" << key << "' pressed; Not supported." << endl;
      break;
    }
}

/* GLUT handles "special" keys through the glutSpecialFunc callback. */
void
graphics::keyboard2 (int key, int x, int y)
{
  switch (key)
    {
    case GLUT_KEY_LEFT: // rotate room left
      yRot += 2; //left rotate
      break;
    case GLUT_KEY_RIGHT: // rotate room right
      glClass->yRot -= 2; //left rotate
      break;
    case GLUT_KEY_UP: // rotate room left
      xRot += 2; //left rotate
      break;
    case GLUT_KEY_DOWN: // rotate room right
      glClass->xRot -= 2; //left rotate
      break;
    default:
      cout << "Key '" << key << "' pressed; Not supported." << endl;
      break;
    }
}

/* Mouse callback function */
void
graphics::mouse (int btn, int state, int x, int y)
{
  int yy = glutGet (GLUT_WINDOW_HEIGHT);
  y = yy - y;

  // only allow selection if game hasn't ended
  if (board::state != board::WIN && board::state != board::LOSE && board::state != board::DRAW)
    {
      if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
          mX = x;
          mY = y;
          glClass->brd->selection (mX, mY);
        }
    }
  //if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  //  {
  //      // currently, menu is attached to right button
  //  }
  if (btn == GLUT_MIDDLE_BUTTON)
    {
      if (state == GLUT_DOWN)
        {
          mouseRotate = true;
          //glutWarpPointer (glClass->width / 2, glClass->height / 2);
        }
      else if (state == GLUT_UP)
        {
          mouseRotate = false;
        }
    }
}

/* Callback function for mouse movement */
void
graphics::motion (int x, int y)
{
  static int oldX = x;
  static int oldY = y;

  if (mouseRotate)
    {
      if ((oldX - x) > 0)
        glClass->yRot += 2;
      else
        glClass->yRot -= 2;
      if ((oldY - y) > 0)
        glClass->xRot += 2;
      else
        glClass->xRot -= 2;
      //cout << "xRot: " << glClass->xRot << endl;
      //cout << "yRot:" << glClass->yRot << endl;
      oldX = x;
      oldY = y;
    }

}

graphics::~graphics ()
{
#ifdef _DEBUG_GENERIC
  cout << "Cleaning up after graphics" << endl;
#endif
}

