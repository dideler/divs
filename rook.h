#ifndef _ROOK_H
#define	_ROOK_H

#include <vector>
#include "unit.h"

using namespace std;

class rook : public unit
{
public:
  GLuint callId; //holds the call list ids for individual pieces
  rook ();
  virtual void drawUnit (int location);
  virtual vector<int> moves (int location, const vector< vector <board::placement> > &boardState);
  virtual GLuint createCallList ();
  virtual ~rook ();
private:

};

#endif	/* _PAWN_H */

