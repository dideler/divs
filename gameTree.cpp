#include <iostream>
#include <cstdlib>
#include "gameTree.h"
#include "unit.h"
#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"

using namespace std;

gameTree::gameTree () { }

gameTree::move
gameTree::findMove (int ply, board::colors currentPlayer, int alpha, int beta,
                    vector< vector <board::placement> > boardState)
{
  gameTree::move tmp;
  gameTree::move maxOrMin;

  if (currentPlayer == board::WHITE)
    {
      tmp.moveValue = -99999;
      maxOrMin.moveValue = -99999;
    }
  else
    {
      tmp.moveValue = +99999;
      maxOrMin.moveValue = +99999;
    }

  if (ply == 0)
    {
      tmp.moveValue = boardScore (boardState);
      return tmp;
    }

  int count = 0;
  for (vector< vector <board::placement> >::iterator i = boardState.begin (); i != boardState.end (); ++i)
    for (vector<board::placement >::iterator j = i->begin (); j != i->end (); ++j, ++count)
      {
        if (j->color == currentPlayer && j->type != board::NONE)
          {
            vector<int> moves = board::units[j->type]->moves (count, boardState);
            for (vector<int>::iterator m = moves.begin (); m != moves.end (); m++)
              {
                if (currentPlayer == board::WHITE) // look for max
                  {
                    tmp = findMove (ply - 1, board::BLACK, alpha, beta, board::movePiece (count, *m, boardState));
                  }
                else // look for min
                  {
                    tmp = findMove (ply - 1, board::WHITE, alpha, beta, board::movePiece (count, *m, boardState));
                  }

                tmp.from = (count);
                tmp.to = *m;

                if (currentPlayer == board::WHITE)
                  {
                    if (tmp.moveValue > maxOrMin.moveValue)
                      {
                        maxOrMin = tmp;
                        if (alpha < maxOrMin.moveValue)
                          alpha = maxOrMin.moveValue;
                      }
                    if (maxOrMin.moveValue > beta)
                      return maxOrMin;
                  }
                else
                  {
                    if (tmp.moveValue < maxOrMin.moveValue)
                      {
                        maxOrMin = tmp;
                        if (beta > maxOrMin.moveValue)
                          beta = maxOrMin.moveValue;
                      }
                    if (maxOrMin.moveValue < alpha)
                      return maxOrMin;
                  }
              }
          }
      }
  return maxOrMin;
}

int
gameTree::boardScore (const vector< vector <board::placement> > &boardState)
{
  float white = 0;
  float black = 0;

  for (vector< vector <board::placement> >::const_iterator i = boardState.begin (); i != boardState.end (); ++i)
    for (vector<board::placement >::const_iterator j = i->begin (); j != i->end (); ++j)
      {
        if (j->color == board::WHITE)
          white += unitValue (j->type);
        else
          black += unitValue (j->type);
      }
  return white - black;
}

int
gameTree::unitValue (board::unitTypes type)
{
  switch (type)
    {
    case board::NONE :
      return 0;
      break;
    case board::PAWN :
      return 1;
      break;
    case board::BISHOP :
      return 3;
      break;
    case board::KNIGHT :
      return 3;
      break;
    case board::ROOK :
      return 5;
      break;
    case board::QUEEN :
      return 8;
      break;
    case board::KING :
      return 95;
      break;
    default:
      return 0;
      break;
    }
}

gameTree::move
gameTree::randomMove (board::colors aiPlayer, const vector< vector <board::placement> > &boardState)
{
  vector<gameTree::move> moves;

  for (int i = 0; i < 64; i++)
    {
      int tmpRow = i / 8;
      int tmpCol = i % 8;
      gameTree::move tmpMove;
      tmpMove.moveValue = 0;
      if ((board::pieces[tmpRow][tmpCol].type == board::KNIGHT ||
           board::pieces[tmpRow][tmpCol].type == board::PAWN) &&
          board::pieces[tmpRow][tmpCol].color == aiPlayer)
        {
          tmpMove.from = i;
          vector<int> moveList = board::units[board::pieces[tmpRow][tmpCol].type]->moves (i, boardState);
          for (vector<int>::iterator m = moveList.begin (); m != moveList.end (); m++)
            {
              tmpMove.to = *m;
              moves.push_back (tmpMove);
            }
        }
    }
  return moves[rand () % moves.size ()];
}

gameTree::status
gameTree::boardState (board::colors currentPlayer, const vector< vector <board::placement> > &boardState)
{

  return CHECK;
}

bool
gameTree::isAttacked (board::colors currentPlayer, const vector< vector <board::placement> > &boardState, int location)
{

  return true;
}

gameTree::~gameTree () { }

