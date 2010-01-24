#ifndef _PAWN_H
#define	_PAWN_H

#include <vector>
#include "unit.h"

using namespace std;

class pawn : public unit
{
public:
  GLuint callId; //holds the call list ids for individual pieces
  pawn ();
  virtual void drawUnit (int location);
  virtual std::vector<int> moves (int location, const vector< vector <board::placement> > &boardState);
  virtual GLuint createCallList ();
  virtual ~pawn ();
private:

};

#endif	/* _PAWN_H */

