#ifndef _KNIGHT_H
#define	_KNIGHT_H

#include <vector>
#include "unit.h"

using namespace std;

class knight : public unit
{
public:
  GLuint callId; //holds the call list ids for individual pieces
  knight ();
  virtual void drawUnit (int location);
  virtual std::vector<int> moves (int location, const vector< vector <board::placement> > &boardState);
  virtual GLuint createCallList ();
  virtual ~knight ();
private:

};

#endif	/* _PAWN_H */

