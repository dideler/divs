//#define _DEBUG_GENERIC

#ifndef _BOARD_H
#define	_BOARD_H

#include <GL/glut.h> // includes glu.h & gl.h
#include <vector>
//#include "unit.h"

class unit;
class gameTree;

using namespace std;

#define WHITE_SQUARE 1.0
#define BLACK_SQUARE 0.4

/**
 * Board class. Implements functions of the chess board
 */
class board
{
public:
  gameTree *ab;
  int lastPick; ///< id of last picked square (the one that will need to move to new location)
  bool firstPick; ///< if its the first pick, we choose the piece to move
  ///< if its not the first pick, we are trying to move it somewhere

  enum colors
  {
    WHITE, BLACK
  }; ///< Possible chess colors

  enum unitTypes
  {
    PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING, NONE
  }; ///< possible chess units

  enum gameState
  {
    WIN, LOSE, DRAW, ONGOING, EDIT, BLACK_IN_CHECK, WHITE_IN_CHECK,
  }; ///< possible game states

  struct placement
  {
    unitTypes type;
    colors color;
    //squares native middle coordinate y is always 0
    int sx;
    int sz;
    //pieces middle coordinate beloning to square location used for animation later
    int ux;
    int uy;
    int uz;
    int moved; //has the unit been moved? 0 no, 1 yes, 2 en-passant possible
    bool highlighted;
    bool isAttacked;
    int wKingLoc;
    int bKingLoc;
  }; ///< data structure for the array representing the board

  struct materials
  {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
  };

  materials dark;
  materials light;
  materials select;

  static colors currentTurn, aiSide; ///< keeps tract of which players turn it is, keeps track of which side the ai is on
  static unit *units[6]; ///< an array of all the different class types
  static vector< vector <placement> > pieces; ///< 2d vector representation of the board
  static int promoteTo; ///< the type of unit your pawn will promote to
  static int state; ///< game state
  GLuint textures[3]; ///< textures storage
  GLuint boardRender, boardSelect;
  GLint hits; ///< number of hits

  /**
   * Board constructor.
   */
  board (board::colors ai);

  /**
   * Sets up the array to store the board,
   * and places units in the appropriate locations
   */
  void initBoard (void);

  /**
   * Is called in initBoard and when resetting board.
   * It places all chess pieces on their starting positions.
   */
  static void placePieces (void);

  /**
   * Loads textures from a file.
   * @param fileName the name of the image file to read
   * @param texNum the texture id to store it in
   */
  void loadTexture (char * fileName, int texNum);

  /**
   * Responsible for drawing the board and the pieces on it.
   * @param mode specifies wheter its drawn to screen, or in select mode
   */
  void drawBoard (GLenum mode);
  /**
   * Renders the board in selection mode and gets a hit list at the mouse
   * location
   * @param mX x coord of mouse
   * @param mY y coord of mouse
   */
  void selection (int mX, int mY);

  /**
   * processes the hits obtained from selection.
   * Makes appropriate moves and passes control to the AI
   * @param hits the number of hits (size of buffer[])
   * @param buffer array containing the hits (selections)
   */
  void processHits (GLint hits, GLuint buffer[]);

  /**
   * Basic move log for human and AI. Basic as in it doesn't use formal chess
   * move notation. Note that the log tells AI moves from the human's board
   * perspective. For instance, if AI moves queen's pawn 1 square forward, it
   * will print D7 to D6, and not E2 to E3 (which it would from AI's perspective)
   */
  void moveLog (int from, int to);

  /**
   * Deletes a piece on given location. Used in edit - delete mode.
   */
  static void deletePiece (int location);

  /**
   * Adds a piece of a given type and colour on a given location. Used in edit
   * - add mode.
   */
  static void addPiece (int location);

  /**
   * Moves a piece from an old location to a new one.
   * In most cases it simply overwrites the old piece
   * (the one taken over) but it also implements en-passant
   * and call promote when needed.
   * @see promote
   * @param from the piece to move
   * @param to the place to move the piece to
   */
  static vector< vector <board::placement> > movePiece (int from, int to, vector< vector <board::placement> > boardState);

  /**
   * Promotes a pawn when it reaches the opposite end of the board
   * @param row the row the pawn to be promoted is in (either 0 or 7)
   * @param col the row the pawn to be promoted is in
   */
  static vector< vector <board::placement> > promote (int row, int col, vector< vector <board::placement> > boardState);

  /**
   * Moves the specified rook for castling
   * @param location the current location of the rook to be moved
   */
  static vector< vector <board::placement> > castle(int location, vector< vector <board::placement> > boardState);

  /**
   * Checks for terminal or near terminal conditions.
   */
  static void goalTest ();

  void setMaterial (materials mat);
  
  /**
   * Checks which squares are attacked.
   */
  static void squaresAttacked (int myColour, vector< vector <board::placement> > &boardState);

  /** Sets the indicated square as attacked.
   */
  static void setAttacked (int row, int col);

  /**
   * Destructor, final cleanups
   */
  virtual ~board ();

  void makeAiMove ();

private:
};

#endif	/* _BOARD_H */

