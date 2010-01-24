#include <iostream>
#include "unit.h"
#include "board.h"

using namespace std;

unit::unit () { }

void
unit::drawUnit (int location) { }

vector<int>
unit::moves (int location, const vector< vector <board::placement> > &boardState)
{
  vector<int> x;
  return x;
}

vector<int>
unit::pawnMoves (int location, const vector< vector <board::placement> > &boardState)
{
  vector<int> moves;
  int i = location / 8;
  int j = location % 8;
  int myColor = boardState[i][j].color;
  int tmp;

  // pawns cannot move backward, thus colour matters in move direction
  //if black we move forward towards row 7
  if (myColor == board::BLACK)
    {
      //check for enpassants first
      if (boardState[i][j + 1].type == board::PAWN &&
          boardState[i][j + 1].color == board::WHITE &&
          boardState[i][j + 1].moved == 2 &&
          boardState[i - 1][j + 1].type == board::NONE)
        moves.push_back ((i + 1) * 8 + j + 1);
      if (boardState[i][j - 1].type == board::PAWN &&
          boardState[i][j - 1].color == board::WHITE &&
          boardState[i][j - 1].moved == 2 &&
          boardState[i - 1][j - 1].type == board::NONE)
        moves.push_back ((i + 1) * 8 + j - 1);

      tmp = i + 1;
      if (tmp <= 7)
        {
          if (boardState[tmp][j].type == board::NONE)
            moves.push_back (j + tmp * 8); //move straigth
          if (j > 0 && boardState[tmp][j - 1].type != board::NONE && boardState[tmp][j - 1].color != myColor)
            moves.push_back (j - 1 + tmp * 8); //attack left
          if (j < 7 && boardState[tmp][j + 1].type != board::NONE && boardState[tmp][j + 1].color != myColor)
            moves.push_back (j + 1 + tmp * 8); //attack right
        }

      if (boardState[tmp][j].type == board::NONE)
        {
          tmp = tmp + 1;
          if (boardState[i][j].moved == 0 && tmp >= 0 && boardState[tmp][j].type == board::NONE)
            {
              //if it hasnt moved yet, add the posibility of moving 2 squares;
              if (boardState[i][j].moved == 0)
                {
                  if (boardState[tmp][j].type == board::NONE)
                    moves.push_back (j + tmp * 8); //move straigth
                }
            }
        }
    }
  else //if white move towards row 0
    {
      //check for enpassants first
      if (boardState[i][j + 1].type == board::PAWN &&
          boardState[i][j + 1].color == board::BLACK &&
          boardState[i][j + 1].moved == 2 &&
          boardState[i - 1][j + 1].type == board::NONE)
        moves.push_back ((i - 1) * 8 + j + 1);
      if (boardState[i][j - 1].type == board::PAWN &&
          boardState[i][j - 1].color == board::BLACK &&
          boardState[i][j - 1].moved == 2 &&
          boardState[i - 1][j - 1].type == board::NONE)
        moves.push_back ((i - 1) * 8 + j - 1);

      tmp = i - 1;
      if (tmp >= 0)
        {
          if (boardState[tmp][j].type == board::NONE)
            moves.push_back (j + tmp * 8); //move straigth
          if (j > 0 && boardState[tmp][j - 1].type != board::NONE && boardState[tmp][j - 1].color != myColor)
            moves.push_back (j - 1 + tmp * 8); //attack left
          if (j < 7 && boardState[tmp][j + 1].type != board::NONE && boardState[tmp][j + 1].color != myColor)
            moves.push_back (j + 1 + tmp * 8); //attack right
        }

      if (boardState[tmp][j].type == board::NONE)
        {
          tmp = tmp - 1;
          if (boardState[i][j].moved == 0 && tmp >= 0 && boardState[tmp][j].type == board::NONE)
            {
              //if it hasnt moved yet, add the posibility of moving 2 squares;
              if (boardState[i][j].moved == 0)
                {
                  if (boardState[tmp][j].type == board::NONE)
                    moves.push_back (j + tmp * 8); //move straigth
                }
            }
        }
    }
  return moves;
}

vector<int>
unit::knightMoves (int location, const vector< vector <board::placement> > &boardState) // moves 2-1 and jumps (8 max possible moves)
{
  vector<int> moves;
  int row = location / 8;
  int col = location % 8;
  int myColor = boardState[row][col].color;

  int movRow[] = {1, 2, 2, 1, -1, -2, -2, -1};
  int movCol[] = {-2, -1, 1, 2, -2, -1, 1, 2};
  int tmpRow, tmpCol;

  for (int i = 0; i < 8; i++)
    {
      tmpRow = row + movRow[i];
      tmpCol = col + movCol[i];

      if (tmpRow >= 0 && tmpRow <= 7 && // check bounds
          tmpCol >= 0 && tmpCol <= 7)
        {
          /*
          // check if empty square or enemy
          if (boardState[tmpRow][tmpCol].type == board::NONE ||
              boardState[tmpRow][tmpCol].color != myColor)
            {
              // if white is checked, player must eliminate threat
             if (board::state == board::WHITE_IN_CHECK && myColor == board::WHITE)
                {
                  moves.push_back (tmpRow * 8 + tmpCol);
                  //board::squaresAttacked (myColor, boardState); // update attacks & refresh state
                  // has player eliminated threat?
                  if (board::state == board::WHITE_IN_CHECK)
                    moves.pop_back ();
                }
              else moves.push_back (tmpRow * 8 + tmpCol); // otherwise can make normal moves
            }
           */
          // empty square or enemy
          if (boardState[tmpRow][tmpCol].type == board::NONE ||
              boardState[tmpRow][tmpCol].color != myColor)
              moves.push_back (tmpRow * 8 + tmpCol);
        }
    }
  return moves;
}

vector<int>
unit::bishopMoves (int location, const vector< vector <board::placement> > &boardState)
{
  vector<int> moves;

  int row = location / 8;
  int col = location % 8;
  int tmpRow, tmpCol;
  int myColor = boardState[row][col].color;

  for (int rowChange = -1; rowChange <= 1; rowChange += 2)
    {
      for (int colChange = -1; colChange <= 1; colChange += 2)
        {
          for (int i = 1; i < 8; i++)
            {
              tmpRow = rowChange * i + row;
              tmpCol = colChange * i + col;

              if (tmpCol <= 7 && tmpCol >= 0 &&
                  tmpRow <= 7 && tmpRow >= 0)
                {
                  if (boardState[tmpRow][tmpCol].type == board::NONE ||
                      boardState[tmpRow][tmpCol].color != myColor)
                    moves.push_back (tmpRow * 8 + tmpCol);

                  // set occupied squares of my own to attacked, so King cannot put
                  // himself in check by trying to capture a piece on such a square
                  if (boardState[tmpRow][tmpCol].color == myColor)
                    board::setAttacked (tmpRow, tmpCol);

                  if (boardState[tmpRow][tmpCol].type != board::NONE)
                    break;
                }
              else
                {
                  break;
                }
            }
        }
    }
  return moves;
}

vector<int>
unit::rookMoves (int location, const vector< vector <board::placement> > &boardState) // can move vertical and horizontal (max 14)
{

  vector<int> moves;

  int row = location / 8;
  int col = location % 8;
  int tmpChange;
  int myColor = boardState[row][col].color;

  for (int change = -1; change <= 1; change += 2)
    {
      for (int i = 1; i < 8; i++)
        {
          tmpChange = change * i + row;
          if (tmpChange <= 7 && tmpChange >= 0)
            {
              //up & down moves
              if (boardState[tmpChange][col].type == board::NONE ||
                  boardState[tmpChange][col].color != myColor)
                moves.push_back (tmpChange * 8 + col);

              // set occupied squares of my own to attacked, so King cannot put
              // himself in check by trying to capture a piece on such a square
              if (boardState[tmpChange][col].color == myColor)
                board::setAttacked (tmpChange, col);

              if (boardState[tmpChange][col].type != board::NONE)
                break;
            }
          else
            {
              break;
            }
        }
    }

  for (int change = -1; change <= 1; change += 2)
    {
      for (int i = 1; i < 8; i++)
        {
          tmpChange = change * i + col;
          if (tmpChange <= 7 && tmpChange >= 0)
            {
              //left & right moves
              if (boardState[row][tmpChange].type == board::NONE ||
                  boardState[row][tmpChange].color != myColor)
                moves.push_back (row * 8 + tmpChange);

              // set occupied squares of my own to attacked, so King cannot put
              // himself in check by trying to capture a piece on such a square
              if (boardState[tmpChange][col].color == myColor)
                board::setAttacked (tmpChange, col);

              if (boardState[row][tmpChange].type != board::NONE)
                break;
            }
          else
            {
              break;
            }
        }
    }
  return moves;
}

vector<int>
unit::kingMoves (int location, const vector< vector <board::placement> > &boardState) // king can move to any adjacent square (max 8)
{
  vector<int> moves;
  int row = location / 8;
  int col = location % 8;
  int myColor = boardState[row][col].color;

  int movRow[] = {1, 1, 1, 0, 0, -1, -1, -1};
  int movCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int tmpRow, tmpCol;

  for (int i = 0; i < 8; ++i)
    {
      tmpRow = row + movRow[i];
      tmpCol = col + movCol[i];

      if (tmpRow >= 0 && tmpRow <= 7 && // check bounds
          tmpCol >= 0 && tmpCol <= 7)
        { // check if empty square or enemy
          if (boardState[tmpRow][tmpCol].type == board::NONE ||
              boardState[tmpRow][tmpCol].color != myColor)
            {
              if (boardState[tmpRow][tmpCol].isAttacked == false)
                moves.push_back (tmpRow * 8 + tmpCol);
            }
        }
    }

  /* Castling can only be done if:
   * 1) the king has never moved
   * 2) the chosen rook has never moved
   * 3) there are no pieces between the king and rook
   * 4) the king is not in check
   * 5) castling doesn't result in the king being in check (i.e. cannot go on an attacked square)
   * 6) the king cannot pass through an attacked square
   * 7) king and rook must be on the same rank (i.e. row) -- this is always the case if rules 1 & 2 are met
   */
  
  // check if castling is possible on either side of the board
  if((location == 60 || location == 4) && boardState[row][col].moved == 0) // has king moved?
    {
      //check if path is empty for castling short
      if(boardState[row][col+1].type == board::NONE
         && boardState[row][col+2].type == board::NONE)
        {
          // does kingside rook meet conditions?
          if(boardState[row][col+3].type == board::ROOK // is rook on its position?
             && boardState[row][col+3].color == myColor // is it my rook?
             && boardState[row][col+3].moved == 0)      // has the rook moved?
            {
              // check if move is safe
              if(boardState[row][col].isAttacked == false // is king in check?
                 && boardState[row][col+1].isAttacked == false // is first square safe?
                 && boardState[row][col+2].isAttacked == false) // is second square safe?
                {
                  // all conditions met, castling short is allowed
                  moves.push_back (row * 8 + col + 2);
                }
            }
        }

      //check if path is empty for castling long
      if(boardState[row][col-1].type == board::NONE
         && boardState[row][col-2].type == board::NONE
         && boardState[row][col-3].type == board::NONE)
        {
          // does queenside rook meet conditions?
          if(boardState[row][col-4].type == board::ROOK // is rook on its position?
             && boardState[row][col-4].color == myColor // is it my rook?
             && boardState[row][col-4].moved == 0)      // has the rook moved?
            {
              // check if move is safe
              if(boardState[row][col].isAttacked == false // is king in check?
                 && boardState[row][col-1].isAttacked == false // is first square safe?
                 && boardState[row][col-2].isAttacked == false) // is second square safe?
                {
                  // all conditions met, castling long is allowed
                  moves.push_back (row * 8 + col - 2);
                }
            }
        }
    }
  
  return moves;
}

vector<int>
unit::queenMoves (int location, const vector< vector <board::placement> > &boardState)
{
  vector<int> moves = rookMoves (location, boardState);
  vector<int> tmp = bishopMoves (location, boardState);
  for (vector<int>::iterator a = tmp.begin (); a != tmp.end (); a++)
    {
      moves.push_back (*a);
    }
  return moves;
}

GLuint
unit::createCallList ()
{
  return NULL;
}

unit::~unit () { }

