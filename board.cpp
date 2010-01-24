#include <iostream>
#include <GL/glut.h> // includes glu.h & gl.h
#include <FreeImage.h>
#include <cstdlib>
#include <ctime>
#include "board.h"
#include "unit.h"
#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"
#include "graphics.h"
#include "gameTree.h"

using namespace std;

unit *board::units[6];
vector<vector <board::placement> > board::pieces;
board::colors board::currentTurn, board::aiSide;
int board::promoteTo, board::state;

void
board::makeAiMove ()
{
  clock_t start = clock ();
  gameTree::move mv = ab->findMove (graphics::plyLevel, aiSide, -99999, 99999, pieces);
  clock_t end = clock ();
  pieces = movePiece (mv.from, mv.to, pieces); // move AI piece
  cout << "(AI took " << (end - start) / (float) CLOCKS_PER_SEC << "s)\n" << endl;
  moveLog (mv.from, mv.to);
}

board::board (board::colors ai)
{
  ab = new gameTree ();
  firstPick = true;
  currentTurn = WHITE;
  aiSide = ai;
  promoteTo = QUEEN;
  state = ONGOING;
  initBoard ();

  //if ai is white, let it start the game
  if (aiSide == WHITE)
    {
      clock_t start = clock ();
      gameTree::move mv = ab->findMove (graphics::plyLevel, aiSide, -99999, 99999, pieces);
      clock_t end = clock ();
      pieces = movePiece (mv.from, mv.to, pieces); // move AI piece
      cout << "(AI took " << (end - start) / (float) CLOCKS_PER_SEC << "s)\n" << endl;
      moveLog (mv.from, mv.to);
    }

  /* Set up textures */
  glGenTextures (3, textures); // create 3 textures
  loadTexture ((char *) "./textures/white.jpg", 0);
  loadTexture ((char *) "./textures/black.jpg", 1);
  loadTexture ((char *) "./textures/wood.jpg", 2);

  light.ambient[0] = 0.5;
  light.ambient[1] = 0.5;
  light.ambient[2] = 0.5;
  light.ambient[3] = 1.0;
  light.diffuse[0] = 0.8;
  light.diffuse[1] = 0.8;
  light.diffuse[2] = 0.8;
  light.diffuse[3] = 1.0;
  light.specular[0] = 1.0;
  light.specular[1] = 1.0;
  light.specular[2] = 1.0;
  light.specular[3] = 1.0;
  light.shininess = 80.8;

  dark.ambient[0] = 0.25;
  dark.ambient[1] = 0.25;
  dark.ambient[2] = 0.25;
  dark.ambient[3] = 1.0;
  dark.diffuse[0] = 0.35;
  dark.diffuse[1] = 0.35;
  dark.diffuse[2] = 0.35;
  dark.diffuse[3] = 1.0;
  dark.specular[0] = 1.0;
  dark.specular[1] = 1.0;
  dark.specular[2] = 1.0;
  dark.specular[3] = 1.0;
  dark.shininess = 80.0;

  select.ambient[0] = 1.0;
  select.ambient[1] = 1.0;
  select.ambient[2] = 0.0;
  select.ambient[3] = 1.0;
  select.diffuse[0] = 1.0;
  select.diffuse[1] = 1.0;
  select.diffuse[2] = 0.0;
  select.diffuse[3] = 1.0;
  select.specular[0] = 1.0;
  select.specular[1] = 1.0;
  select.specular[2] = 0.0;
  select.specular[3] = 1.0;
  select.shininess = 80.0;
}

void
board::initBoard ()
{
  placement tmp;
  tmp.color = WHITE;
  tmp.type = NONE;

  //initalize 8x8 board structure for keeping track of the chess pieces
  for (int x = 0; x < 8; x++)
    {
      pieces.push_back (vector<placement > ());
      for (int y = 0; y < 8; y++)
        {
          tmp.sx = x * 5.5 + 2.25;
          tmp.sz = x * 5.5 + 2.25;
          tmp.ux = tmp.sx;
          tmp.uy = 0;
          tmp.uz = tmp.sz;
          tmp.isAttacked = false;
          tmp.moved = 0;
          tmp.highlighted = false;
          pieces[x].push_back (tmp);
        }
    }

  placePieces (); // place the chess pieces on the board
}

void
board::placePieces ()
{
  /*
  #ifdef _DEBUG_GENERIC
    cout << "initial board configuration \"type color\"" << endl;
  #endif
   */
  //place black and white pawns
  for (int i = 0; i < 8; i++)
    {
      pieces[1][i].type = PAWN;
      pieces[6][i].type = PAWN;
      pieces[1][i].color = BLACK;
      pieces[6][i].color = WHITE;
    }

  //place knights
  pieces[0][1].type = KNIGHT;
  pieces[7][1].type = KNIGHT;
  pieces[0][1].color = BLACK;
  pieces[7][1].color = WHITE;
  pieces[0][6].type = KNIGHT;
  pieces[7][6].type = KNIGHT;
  pieces[0][6].color = BLACK;
  pieces[7][6].color = WHITE;

  //place rooks
  pieces[0][0].type = ROOK;
  pieces[7][0].type = ROOK;
  pieces[0][0].color = BLACK;
  pieces[7][0].color = WHITE;
  pieces[0][7].type = ROOK;
  pieces[7][7].type = ROOK;
  pieces[0][7].color = BLACK;
  pieces[7][7].color = WHITE;

  //place bishops
  pieces[0][2].type = BISHOP;
  pieces[7][2].type = BISHOP;
  pieces[0][2].color = BLACK;
  pieces[7][2].color = WHITE;
  pieces[0][5].type = BISHOP;
  pieces[7][5].type = BISHOP;
  pieces[0][5].color = BLACK;
  pieces[7][5].color = WHITE;

  //place queens
  pieces[0][3].type = QUEEN;
  pieces[7][3].type = QUEEN;
  pieces[0][3].color = BLACK;
  pieces[7][3].color = WHITE;

  //place kings
  pieces[0][4].type = KING;
  pieces[7][4].type = KING;
  pieces[0][4].color = BLACK;
  pieces[7][4].color = WHITE;

  //place king locations
  pieces[0][0].wKingLoc = 60;
  pieces[0][0].bKingLoc = 4;

  //initalize units
  units[PAWN] = new pawn ();
  units[BISHOP] = new bishop ();
  units[KNIGHT] = new knight ();
  units[ROOK] = new rook ();
  units[QUEEN] = new queen ();
  units[KING] = new king ();
  /*
  #ifdef _DEBUG_GENERIC
    for (int i = 0; i < 8; i++)
      {
        for (int j = 0; j < 8; j++)
          {
            cout << pieces[i][j].type << " " << pieces[i][j].color << '\t';
          }
        cout << endl;
      }
  #endif
   */
}

/* All textures that we are using are textures that we found online. */
void
board::loadTexture (char * fileName, int texNum)
{
  // Get the image file type
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType (fileName, 0);

  // Load the image; exit on failure
  FIBITMAP * texture;
  if ((texture = FreeImage_Load (format, fileName, 0)) == NULL)
    {
      cerr << "FreeImage_Load" << endl;
      exit (0);
    }

  glBindTexture (GL_TEXTURE_2D, textures[texNum]);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width = FreeImage_GetWidth (texture);
  int height = FreeImage_GetHeight (texture);

  BYTE * imgPtr = (BYTE *) FreeImage_GetBits (texture); // pointer to image data

  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imgPtr);

  FreeImage_Unload (texture); // Unload the image
}

/* Draws an 8x8 board by repeating a square 64 times; alternating
 * between white and black. */
void
board::drawBoard (GLenum mode)
{
  //Anti aliasing commented out, seems like lab computers cant handle it
  //Yay for software rendering
  /*
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST); // Set Line Antialiasing
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST); // Set Line Antialiasing
  glEnable (GL_POLYGON_SMOOTH);
  glEnable (GL_LINE_SMOOTH);
  */ 
  glEnable (GL_LIGHT0);
  glLightfv (GL_LIGHT0, GL_POSITION, graphics::lightPosition);

  GLuint id = 0, sqrCol = 0; // each square has an ID and a colour

  if (graphics::textures) glEnable (GL_TEXTURE_2D);
  else glDisable (GL_TEXTURE_2D);

  for (float i = 0; i < 8 * 5.5; i += 5.5, ++sqrCol) // rows
    {

      for (float j = 0; j < 8 * 5.5; j += 5.5, id++, ++sqrCol) // columns
        {
          if (sqrCol % 2 == 1)
            {
              glColor3f (0.4, 0.4, 0.4);
              setMaterial (dark);
              glBindTexture (GL_TEXTURE_2D, textures[1]);
            }
          else
            {
              glColor3f (1.0, 1.0, 1.0);
              setMaterial (light);
              glBindTexture (GL_TEXTURE_2D, textures[0]);
            }

          if (mode == GL_SELECT) glPushName (id);

          if (board::pieces[id / 8][id % 8].highlighted == true)
            {
              if (graphics::texBlend)
                {
                  glEnable (GL_BLEND);
                  setMaterial (select);
                  glColor4f (0.7, 0.7, 0.0, 0.1);
                }
              else glDisable (GL_TEXTURE_2D); // in order to see highlighting
            }

          glEnable (GL_CULL_FACE);
          glCullFace (GL_BACK);
          glBegin (GL_TRIANGLES);

          glNormal3f (0.0, 1.0, 0.0);
          glTexCoord2f (0.0, 0.0);
          glVertex3f (j, 0, i);

          glNormal3f (0.0, 1.0, 0.0);
          glTexCoord2f (0.0, 1.0);
          glVertex3f (j, 0, i + 5.5);

          glNormal3f (0.0, 1.0, 0.0);
          glTexCoord2f (1.0, 0.0);
          glVertex3f (j + 5.5, 0, i);


          glNormal3f (0.0, 1.0, 0.0);
          glTexCoord2f (1.0, 0.0);
          glVertex3f (j + 5.5, 0, i);

          glNormal3f (0.0, 1.0, 0.0);
          glTexCoord2f (0.0, 1.0);
          glVertex3f (j, 0, i + 5.5);

          glNormal3f (0.0, 1.0, 0.0);
          glTexCoord2f (1.0, 1.0);
          glVertex3f (j + 5.5, 0, i + 5.5);

          glEnd ();
          glDisable (GL_CULL_FACE);

          if (mode == GL_SELECT) glPopName ();

          if (board::pieces[id / 8][id % 8].highlighted == true)
            {
              if (graphics::texBlend) glDisable (GL_BLEND);
              else glEnable (GL_TEXTURE_2D);
            }
        }
    }

  glColor3f (0.7, 0.7, 0.7); // draw wooden base a bit brighter & prevent highlighting


  setMaterial (dark);
  /* Draw the wooden part which the board rests on.
   * Can later try putting code in a loop (current way is faster but dirtier) */
  glBindTexture (GL_TEXTURE_2D, textures[2]);
  glEnable (GL_CULL_FACE);

  //NOTE: textures behave funny on angled planes when rotated
  glCullFace (GL_FRONT);
  glBegin (GL_QUADS);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (-2, -1, -2); // bottom back base
  glTexCoord2f (0.0, 1.0);
  glVertex3f (-2, -5, -2);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (46, -5, -2);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (46, -1, -2);

  glTexCoord2f (1.0, 1.0);
  glVertex3f (46, -5, 46); // bottom right base
  glTexCoord2f (0.0, 1.0);
  glVertex3f (46, -1, 46);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (46, -1, -2);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (46, -5, -2);

  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (0, 0, 0); // top left base
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (0, 0, 44);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (-2, -1, 46);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (-2, -1, -2);

  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (0, 0, 0); // top back base
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (-2, -1, -2);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (46, -1, -2);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (44, 0, 0);
  glEnd ();

  glCullFace (GL_BACK);
  glBegin (GL_QUADS);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (-2, -5, 46); // bottom front base
  glTexCoord2f (0.0, 1.0);
  glVertex3f (46, -5, 46);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (46, -1, 46);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (-2, -1, 46);

  glTexCoord2f (1.0, 0.0);
  glVertex3f (-2, -5, -2); // bottom left base
  glTexCoord2f (1.0, 1.0);
  glVertex3f (-2, -5, 46);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (-2, -1, 46);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (-2, -1, -2);

  glNormal3f (0.0, -1.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (46, -5, -2); // bottom base
  glNormal3f (0.0, -1.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (46, -5, 46);
  glNormal3f (0.0, -1.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (-2, -5, 46);
  glNormal3f (0.0, -1.0, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (-2, -5, -2);

  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (0, 0, 44); // top front base
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (-2, -1, 46);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (46, -1, 46);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (44, 0, 44);

  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (44, 0, 44); // top right base
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (46, -1, 46);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (46, -1, -2);
  glNormal3f (0.0, 1.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (44, 0, 0);
  glEnd ();

  glDisable (GL_CULL_FACE);

  if (graphics::marble) glBindTexture (GL_TEXTURE_2D, textures[1]);

  //draw units on chessboard

  glEnable (GL_CULL_FACE);
  glCullFace (GL_BACK);

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      {
        glPushMatrix ();
        glTranslatef (j * 5.5 + 2.75, 0, i * 5.5 + 2.75);
        if (pieces[i][j].type != NONE)
          {
            if (board::pieces[i][j].color == board::BLACK)
              {
                glColor3f (0.2, 0.2, 0.2);
                setMaterial (dark);
              }
            else
              {
                glColor3f (0.8, 0.8, 0.8);
                setMaterial (light);
                glRotatef (180, 0, 1, 0); // rotate unit to face forward
              }
            units[pieces[i][j].type]->drawUnit (i * 8 + j);
          }
        glPopMatrix ();
      }

  glDisable (GL_CULL_FACE);
}

/* Selection/Picking: used to detect if a user has clicked on an object.  */
void
board::selection (int mX, int mY)
{
  GLuint nameBuff[100];
  GLint viewport[4]; // used to set clipping volume for picking

  /* Init the name stack & set up viewing port for a desired region */
  glGetIntegerv (GL_VIEWPORT, viewport);
  glSelectBuffer (100, nameBuff); // identify array for the selection data
  glRenderMode (GL_SELECT); // enter selection mode

  glInitNames ();

  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();

  // Create a 1x1 pixel picking region near cursor location
  gluPickMatrix ((GLdouble) mX, (GLdouble) mY, 1.0, 1.0, viewport);
  gluPerspective (40.0, (GLfloat) graphics::width / (GLfloat) graphics::height, 1.0, 300.0);

  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  graphics::rotateBoard ();

  drawBoard (GL_SELECT);

  hits = glRenderMode (GL_RENDER); // return to normal render mode + return hits
  processHits (hits, nameBuff);

  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glutSwapBuffers ();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
}

/* This function examines the name stack.
 * The stack contains 1 record for each hit. Each record contains 3 types of
 * information: 1) number of names on the stack when there was a hit.
 * 2) scaled minimum and max depths for the hit primitive (useful in 3D apps).
 * 3) entries in the name stack, to identify which object was hit.  */
void
board::processHits (GLint hits, GLuint buffer[])
{
  int clicked = -1;
  GLuint names, * ptr;

  //cout << "\nhits = " << hits << endl;

  ptr = (GLuint *) buffer;

  for (int i = 0; i < hits; ++i) // loop over number of hits
    {
      names = *ptr;
      //cout << "number of names for this hit = " << names << endl;
      ptr++;
      //cout << "z1 = " << (float) * ptr / 0x7fffffff << endl;
      ptr++;
      //cout << "z2 = " << (float) * ptr / 0x7fffffff << endl;
      ptr++;

      for (unsigned int j = 0; j < names; ++j) // check each name in record
        {
          clicked = *ptr;
          //cout << "square " << clicked << endl;
          if (graphics::addMode) // if in edit->add mode, add a certain piece
            {
              addPiece (clicked);
              return;
            }
          else if (graphics::deleteMode) // if in edit->delete mode, delete the piece
            {
              deletePiece (clicked);
              return;
            }
          ptr++; // go to next hit record
        }
    }
  if (hits == 1 && names == 0) return; // prevent pieces from going out of bounds
  if (hits > 0)
    {
      if (firstPick) //if its the first pick, user is choosing unit to move
        {
          unitTypes piece = pieces[clicked / 8][clicked % 8].type;
          colors pieceColour = pieces[clicked / 8][clicked % 8].color;
          if (piece != NONE && pieceColour != aiSide) // check if piece valid
            {
              vector<int> moveslist = units[piece]->moves (clicked, pieces); // generate moves
              for (vector<int>::iterator i = moveslist.begin (); i != moveslist.end (); i++)
                {
                  //cout << "can move to square " << *i << endl;
                  pieces[*i / 8][*i % 8].highlighted = true; // highlight possible moves
                }
              pieces[clicked / 8][clicked % 8].highlighted = true; // hightlight chosen piece

              lastPick = clicked;
              firstPick = false;
            }
        }
      else //otherwise its second pick, user has chosen where to move previously selected piece
        {
          // check if the selected move is a possible option
          if (pieces[clicked / 8][clicked % 8].highlighted == true && clicked != lastPick)
            {
              pieces = movePiece (lastPick, clicked, pieces); // move human piece


              /* Reset some board properties */
              colors lastColor = pieces[lastPick / 8][lastPick % 8].color;
              for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                  {
                    pieces[i][j].highlighted = false; // reset highlighting
                    pieces[i][j].isAttacked = false; // reset attacked status

                    if (lastColor == BLACK) //reset moved values of opponent from 2 so on next move player enpassant is no longer possible on next turn
                      {
                        if (pieces[i][j].moved == 2 && pieces[i][j].color == WHITE)
                          pieces[i][j].moved = 1;
                      }
                    else
                      {
                        if (pieces[i][j].moved == 2 && pieces[i][j].color == BLACK)
                          pieces[i][j].moved = 1;
                      }
                  }

              squaresAttacked (aiSide, pieces); // update attacks against AI

              if (!firstPick && lastPick != clicked)
                {
                  moveLog (lastPick, clicked);
                }

              // check if multiplayer mode -- change turn, rotate view, change player colour
              if (graphics::twoPlayer)
                {
                  currentTurn == WHITE ? currentTurn = BLACK : currentTurn = WHITE;
                  if (currentTurn == BLACK)
                    {
                      graphics::yRot = 360;
                      aiSide = WHITE;
                    }
                  if (currentTurn == WHITE)
                    {
                      graphics::yRot = 0;
                      aiSide = BLACK;
                    }
                }

                // only call AI if game is in play
              else if (state == ONGOING || state == WHITE_IN_CHECK || state == BLACK_IN_CHECK)
                {
                  clock_t start = clock ();
                  gameTree::move mv = ab->findMove (graphics::plyLevel, aiSide, -99999, 99999, pieces);
                  if (mv.moveValue == 0)
                    mv = ab->randomMove (aiSide, pieces);
                  clock_t end = clock ();
                  if (mv.from >= 0 && mv.from <= 64) //only move if valid move was found
                    {
                      pieces = movePiece (mv.from, mv.to, pieces); // move AI piece

                      /* Reset some board properties */
                      colors humanColour;
                      aiSide == BLACK ? humanColour = WHITE : humanColour = BLACK;

                      for (int i = 0; i < 8; i++)
                        for (int j = 0; j < 8; j++)
                          {
                            pieces[i][j].isAttacked = false; // reset attacked status

                            if (pieces[i][j].moved == 2 && pieces[i][j].color == humanColour)
                              pieces[i][j].moved = 1; // disallow en passant on next turn
                          }

                      squaresAttacked (humanColour, pieces); // update attacks against human

                      moveLog (mv.from, mv.to);
                      cout << "(AI took " << (end - start) / (float) CLOCKS_PER_SEC << "s)\n" << endl;
                    }
                  else
                    {
                      state = WIN;
                    }
                }
            }
          else // square clicked is not a possible option, undo possible moves
            {
              for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                  {
                    pieces[i][j].highlighted = false;
                  }
            }

          firstPick = true;
        }
    }
}

void
board::moveLog (int from, int to)
{
  if (pieces[to / 8][to % 8].color == WHITE) cout << "White moved ";
  else cout << "Black moved ";

  unitTypes piece = pieces[to / 8][to % 8].type;
  switch (piece)
    {
    case NONE:
      break;
    case PAWN:
      cout << "PAWN from ";
      break;
    case BISHOP:
      cout << "BISHOP from ";
      break;
    case KNIGHT:
      cout << "KNIGHT from ";
      break;
    case ROOK:
      cout << "ROOK from ";
      break;
    case QUEEN:
      cout << "QUEEN from ";
      break;
    case KING:
      cout << "KING from ";
      break;
    }

  char fromCol = (char) (from % 8) + 65;
  int fromRow = 8 - (from / 8);
  char toCol = (char) (to % 8) + 65;
  int toRow = 8 - (to / 8);

  cout << fromCol << "" << fromRow << " to " << toCol << "" << toRow << endl;
}

void
board::deletePiece (int location)
{
  int i = location / 8;
  int j = location % 8;
  pieces[i][j].type = NONE;
}

void
board::addPiece (int location)
{
  int i = location / 8;
  int j = location % 8;
  if (pieces[i][j].type == NONE)
    {
      switch (graphics::addUnit)
        {
        case graphics::M_W_PAWN :
                  pieces[i][j].type = PAWN;
          pieces[i][j].color = WHITE;
          break;
        case graphics::M_W_BISHOP :
                  pieces[i][j].type = BISHOP;
          pieces[i][j].color = WHITE;
          break;
        case graphics::M_W_KNIGHT :
                  pieces[i][j].type = KNIGHT;
          pieces[i][j].color = WHITE;
          break;
        case graphics::M_W_ROOK :
                  pieces[i][j].type = ROOK;
          pieces[i][j].color = WHITE;
          break;
        case graphics::M_W_QUEEN :
                  pieces[i][j].type = QUEEN;
          pieces[i][j].color = WHITE;
          break;
        case graphics::M_W_KING :
                  pieces[i][j].type = KING;
          pieces[i][j].color = WHITE;
          break;
        case graphics::M_B_PAWN :
                  pieces[i][j].type = PAWN;
          pieces[i][j].color = BLACK;
          break;
        case graphics::M_B_BISHOP :
                  pieces[i][j].type = BISHOP;
          pieces[i][j].color = BLACK;
          break;
        case graphics::M_B_KNIGHT :
                  pieces[i][j].type = KNIGHT;
          pieces[i][j].color = BLACK;
          break;
        case graphics::M_B_ROOK :
                  pieces[i][j].type = ROOK;
          pieces[i][j].color = BLACK;
          break;
        case graphics::M_B_QUEEN :
                  pieces[i][j].type = QUEEN;
          pieces[i][j].color = BLACK;
          break;
        case graphics::M_B_KING :
                  pieces[i][j].type = KING;
          pieces[i][j].color = BLACK;
          break;
        default:
          break;
        }
    }
}

vector< vector <board::placement> >
board::movePiece (int from, int to, vector< vector <board::placement> > boardState)
{
  int toi = to / 8;
  int toj = to % 8;
  int fromi = from / 8;
  int fromj = from % 8;
  int wKing = boardState[0][0].wKingLoc;
  int bKing = boardState[0][0].bKingLoc;

  // check for en passant
  if (boardState[fromi][fromj].type == PAWN && fromj - toj != 0 && boardState[toi][toj].type == NONE)
    boardState[fromi][fromj + (toj - fromj)].type = NONE;

  // check for castling
  if (boardState[fromi][fromj].type == KING && (toj - fromj == 2 || toj - fromj == -2))
    boardState = castle (to, boardState); // move the rook

  // If pawned moved two, then moved = 2, otherwise its just a regular move
  if (boardState[fromi][fromj].type == board::PAWN && abs (fromi - toi) == 2)
    boardState[toi][toj].moved = 2;
  else
    boardState[toi][toj].moved = 1;

  boardState[toi][toj].color = boardState[fromi][fromj].color;
  boardState[toi][toj].type = boardState[fromi][fromj].type;
  boardState[fromi][fromj].type = NONE;

  //check for promotion of pawn
  if (boardState[toi][toj].type == PAWN && boardState[toi][toj].color == WHITE
      && toi == 0) boardState = promote (toi, toj, boardState);

  if (boardState[toi][toj].type == PAWN && boardState[toi][toj].color == BLACK
      && toi == 7) boardState = promote (toi, toj, boardState);

  //preserve king locations in case unit at 0,0 was moved
  boardState[0][0].wKingLoc = wKing;
  boardState[0][0].bKingLoc = bKing;

  //update king locations if they were moved
  if (boardState[toi][toj].type == KING && boardState[toi][toj].color == BLACK)
    boardState[0][0].bKingLoc = toi * 8 + toj;
  if (boardState[toi][toj].type == KING && boardState[toi][toj].color == WHITE)
    boardState[0][0].wKingLoc = toi * 8 + toj;

  return boardState;
}

vector< vector <board::placement> >
board::promote (int row, int col, vector< vector <board::placement> > boardState)
{
  boardState[row][col].type = (unitTypes) promoteTo;
  return boardState;
}

vector< vector <board::placement> >
board::castle (int location, vector< vector <board::placement> > boardState)
{
  int row = location / 8;
  int col = location % 8;
  int myColor = boardState[row][col].color;

  switch (location)
    {
    case 6: // castle short
    case 62:
      boardState[row][col + 1].type = NONE;
      boardState[row][col - 1].type = ROOK;
      boardState[row][col - 1].color = (colors) myColor;
      break;
    case 2: // castle long
    case 58:
      boardState[row][col - 2].type = NONE;
      boardState[row][col + 1].type = ROOK;
      boardState[row][col + 1].color = (colors) myColor;
      break;
    }
  return boardState;
}

void
board::goalTest ()
{
  /* check is when a move threatens an opposing king with capture
   * the player who is in check MUST eliminate the threat by either
   * 1) move the king to a safe square
   * 2) capture the threatening piece
   * 3) sacrifice a piece by placing it in the line of attack
   */
  state = ONGOING; // reset status (in case king is safe)

  // go through every square to find locations of both kings
  int square = 0, whiteKing, blackKing;
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++, square++)
      {

        //      the king will be in "check" because that square is attacked. but doesn't
        if (pieces[row][col].type == KING && pieces[row][col].color == WHITE
            && pieces[row][col].isAttacked)
          {
            state = WHITE_IN_CHECK;
            whiteKing = square;
          }

        if (pieces[row][col].type == KING && pieces[row][col].color == BLACK
            && pieces[row][col].isAttacked)
          {
            state = BLACK_IN_CHECK;
            //TODO might be better to print messages directly instead of using writeMsgs?
            //graphics::glPrint ((char *)"Black checked!");
            blackKing = square;
          }
      }

  /* checkmate is when the king is placed in check and there is no legal move
   * that the player can make to escape check -- thus the king under attack
   * cannot avoid capture.
   */
  //FIXME currently checkmate is not fully working because forcing user to
  //      get rid of check isn't working, which is needed for checkmate
  //      (i.e. needs to check if user cannot get rid of check)
  //FIXME this has the same bug as discovered when restricting pieces to make certain moves
  //      when player is in check. the bug is that the state is set to
  //      IN_CHECK in this local function (even though no player is in check),
  //      while in other functions, the state is still in ON_GOING (correct)

  //if (state == WHITE_IN_CHECK)
  //  {
  //    vector<int> moveList = units[KING]->moves (whiteKing, pieces); // generate moves
  //    if (moveList.empty () /* && no piece can be sacrificed && attacking piece cannot be captured*/)
  //      {
  //        aiSide == BLACK ? state = LOSE : state = WIN;
  //      }
  //  }
  //if (state == BLACK_IN_CHECK)
  //  {
  //    vector<int> moveList = units[KING]->moves (blackKing, pieces); // generate moves
  //    if (moveList.empty () /* && no piece can be sacrificed && attacking piece cannot be captured*/)
  //      {
  //        aiSide == WHITE ? state = LOSE : state = WIN;
  //      }
  //  }
}

void
board::setMaterial (materials mat)
{
  glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
  glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, &mat.shininess);
}

void //Note: myColour doesn't mean human's colour, but the current turn's colour
board::squaresAttacked (int myColour, vector<vector<placement > > &boardState)
{
  int square = 0, tmpRow, tmpCol, kingRow = -1, kingCol = -1;
  unitTypes piece;
  colors pieceColour;

  // need to find my king before we look at which squares are attacked
  for (int row = 0; row < 8; ++row)
    for (int col = 0; col < 8; ++col)
      {
        piece = boardState[row][col].type;
        pieceColour = boardState[row][col].color;
        boardState[row][col].isAttacked = false;

        if (piece == KING && pieceColour == myColour) // find my king location
          {
            kingRow = row;
            kingCol = col;
          }
      }
  if (kingRow == -1)
    {
      if (myColour == aiSide)
        state = WIN;
      else
        state = LOSE;
    }
  else
    {
      for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col, ++square)
          {
            piece = boardState[row][col].type;
            pieceColour = boardState[row][col].color;

            if (piece != NONE && pieceColour != myColour) // find enemy pieces
              {
                if (piece == PAWN) // special case: moves forward, attacks diagonal
                  {
                    if (pieceColour == BLACK) // pawn moves towards row 7
                      {
                        if (col > 0) boardState[row + 1][col - 1].isAttacked = true;
                        if (col < 7) boardState[row + 1][col + 1].isAttacked = true;
                      }
                    else // pawn must be WHITE, moves towards row 0
                      {
                        if (col > 0) boardState[row - 1][col - 1].isAttacked = true;
                        if (col < 7) boardState[row - 1][col + 1].isAttacked = true;
                      }
                  }
                else // piece is another type of unit
                  {
                    // look for pieces that can have a line of attack
                    if (piece == ROOK || piece == BISHOP || piece == QUEEN)
                      {
                        // make king temporarily disappear so that pieces have a better line of attack
                        boardState[kingRow][kingCol].type = NONE;
                      }

                    vector<int> moveslist = units[piece]->moves (square, boardState); // generate moves
                    for (vector<int>::iterator i = moveslist.begin (); i != moveslist.end (); ++i)
                      {
                        tmpRow = *i / 8;
                        tmpCol = *i % 8;

                        //cout <<"square "<<square<<" attacks square: " << *i << endl;
                        boardState[tmpRow][tmpCol].isAttacked = true;
                      }



                    // look for pieces that can have a line of attack
                    if (piece == ROOK || piece == BISHOP || piece == QUEEN)
                      {
                        // replace king
                        boardState[kingRow][kingCol].type = KING;
                        boardState[kingRow][kingCol].color = (colors) myColour;
                      }
                  }
              }
          }

      goalTest ();
    }
}

void
board::setAttacked (int row, int col)
{
  pieces[row][col].isAttacked = true;
}

board::~board ()
{
  delete(ab);
  delete(units[PAWN]);
  delete(units[BISHOP]);
  delete(units[KNIGHT]);
  delete(units[ROOK]);
  delete(units[QUEEN]);
  delete(units[KING]);

  for (int x = 0; x < 8; x++)
    {
      for (int y = 0; y < 8; y++)
        {
          pieces[x].clear ();
        }
      pieces.clear ();
    }

  glDeleteTextures (3, textures);
}

