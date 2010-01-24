//DEBUG FLAGS GO HERE
#define _DEBUG_GENERIC

#ifndef _GRAPHICS_H
#define	_GRAPHICS_H

//###################
#include <GL/glut.h>

class board;

class graphics
{
private:
  static board* brd;
  static GLboolean lightSwitch, mouseRotate, editMode;
  static GLuint fps, frameDelay, base;

  static graphics* glClass;

public:
  static GLfloat lightPosition[4];
  static GLboolean textures, texBlend, deleteMode, addMode, marble, twoPlayer;
  static GLint mX, mY, yRot, xRot;
  static GLuint width, height, plyLevel;

  enum drawType
  {
    WIREFRAME, POINTS, SOLID
  }; static drawType drawMode;

  enum menuOptions
  {
    M_NONE, M_PLY, M_LIGHT, M_TEXTURE, M_SHADOWS, M_CLEAR, M_EDIT, M_MULTIPLAYER,
    M_REFLECTION, M_POSITIONAL, M_DIRECTIONAL, M_QUIT, M_PLY_1, M_PLY_2,
    M_PLY_3, M_PLY_4, M_PLY_5, M_PLY_6, M_PLY_7, M_TEX_BLEND, M_RESET, M_RESIGN,
    M_W_PAWN, M_W_BISHOP, M_W_KNIGHT, M_W_ROOK, M_W_QUEEN, M_W_KING, M_SOLO,
    M_B_PAWN, M_B_BISHOP, M_B_KNIGHT, M_B_ROOK, M_B_QUEEN, M_B_KING,
    M_DELETE, M_P_QUEEN, M_P_ROOK, M_P_BISHOP, M_P_KNIGHT, M_TOGGLE_PIECES,
    M_AI_BLACK, M_AI_WHITE,
  }; static menuOptions addUnit;

  graphics (int argc, char** argv);
  void init (void);
  void buildFont (void);
  void killFont (void);
  static void glPrint (char * text);
  static void writeMsgs (void);
  static void animator (int delay);
  static void display (void);
  static void rotateBoard(void);
  static void reshape (int w, int h);
  void initMenu (void);
  static void menu (int);
  static void keyboard1 (unsigned char key, int x, int y);
  static void keyboard2 (int key, int x, int y);
  static void mouse (int btn, int state, int x, int y);
  static void motion (int x, int y);
  virtual ~graphics ();
};

#endif	/* _GRAPHICS_H */

