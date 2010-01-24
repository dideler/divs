#ifndef _KING_H
#define	_KING_H

#include <vector>
#include "unit.h"

using namespace std;

class king : public unit
{
public:
  GLuint callId; //holds the call list ids for individual pieces
  king ();
  virtual void drawUnit (int location);
  virtual std::vector<int> moves (int location, const vector< vector <board::placement> > &boardState);
  virtual GLuint createCallList ();
  virtual ~king ();
private:

};

#endif	/* _PAWN_H */

