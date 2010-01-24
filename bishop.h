#ifndef _BISHOP_H
#define	_BISHOP_H

#include <vector>
#include "unit.h"

using namespace std;

/**
 * The bishop class. It implements the movements of the bishop
 */
class bishop : public unit
{
public:

  /**
   * Constructor. Sets up the call list.
   */
  bishop ();

  /**
   * Draws a pawn at the specified board location
   * @param location the location where the pawn is to  be drawn
   */
  virtual void drawUnit (int location);

  /**
   * Generates a vector of possible moves for a given location
   * @param location the location we want a list of possible moves for
   * @return list of possible moves from location
   */
  virtual std::vector<int> moves (int location, const vector< vector <board::placement> > &boardState);

  /*
   * Destructor
   */
  virtual ~bishop ();

private:
  GLuint callId; ///< Call list id for drawing the unit
  virtual GLuint createCallList ();
};

#endif

