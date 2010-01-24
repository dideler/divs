#ifndef _QUEEN_H
#define	_QUEEN_H

#include <vector>
#include "unit.h"

using namespace std;

class queen : public unit
{
public:
  GLuint callId; //holds the call list ids for individual pieces
  queen ();
  virtual void drawUnit (int location);
  virtual std::vector<int> moves (int location, const vector< vector <board::placement> > &boardState);
  virtual GLuint createCallList ();
  virtual ~queen ();
private:

};

#endif	/* _PAWN_H */

