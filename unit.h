#ifndef _UNIT_H
#define	_UNIT_H

#include <vector>
#include <GL/glut.h>
#include "board.h"

using namespace std;

class unit
{
public:

  unit ();
  virtual void drawUnit (int location);
  virtual vector<int> moves (int location, const vector< vector <board::placement> > &boardState);
  vector<int> pawnMoves (int location, const vector< vector <board::placement> > &boardState);
  vector<int> knightMoves (int location, const vector< vector <board::placement> > &boardState);
  vector<int> bishopMoves (int location, const vector< vector <board::placement> > &boardState);
  vector<int> rookMoves (int location, const vector< vector <board::placement> > &boardState);
  vector<int> kingMoves (int location, const vector< vector <board::placement> > &boardState);
  vector<int> queenMoves (int location, const vector< vector <board::placement> > &boardState);
  virtual GLuint createCallList ();
  virtual ~unit ();
protected:

};

#endif	/* _UNIT_H */

