#ifndef _GAMETREE_H
#define	_GAMETREE_H

#include <vector>
#include "board.h"

using namespace std;

class gameTree
{
public:

  struct move
  {
    int from, to;
    int moveValue;
  };

  enum status { CHECK, CHECKMATE, STALEMATE};

  gameTree ();
  move findMove (int ply, board::colors currentPlayer, int alpha, int beta, vector< vector <board::placement> > boardState);
  int boardScore (const vector< vector <board::placement> > &boardState);
  inline int unitValue (board::unitTypes type);
  move randomMove (board::colors aiPlayer, const vector< vector <board::placement> > &boardState);
  status boardState(board::colors currentPlayer, const vector< vector <board::placement> > &boardState);
  bool isAttacked (board::colors currentPlayer, const vector< vector <board::placement> > &boardState, int location);
  virtual ~gameTree ();
private:

};

#endif	/* _GAMETREE_H */

