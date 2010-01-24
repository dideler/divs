#include <iostream>
#include "models/pawnModel.h"
#include "pawn.h"
#include "board.h"
#include "graphics.h"

using namespace std;

struct Obj
{
  Point3 *vertices;
  long *v_idx;
  Point3 *normals;
  long *n_idx;
  Point2 *uvs;
  int num_faces;
};

Obj pawnModel;

pawn::pawn ()
{
  pawnModel.vertices = Cylinder0444_vertex;
  pawnModel.v_idx = Cylinder0444_vidx;
  pawnModel.normals = Cylinder0444_normal;
  pawnModel.n_idx = Cylinder0444_nidx;
  pawnModel.uvs = Cylinder0444_uv;
  pawnModel.num_faces = (sizeof (Cylinder0444_vidx) / sizeof (long));

  callId = createCallList ();
}

void
pawn::drawUnit (int location)
{
  glBegin (GL_TRIANGLES);
  glCallList (callId); // draw the precompiled pawn
  glEnd ();
}

vector<int>
pawn::moves (int location, const vector< vector <board::placement> > &boardState)
{
  return pawnMoves (location, boardState);
}

GLuint
pawn::createCallList ()
{
  GLuint listID = glGenLists (1);
  glNewList (listID, GL_COMPILE); // Begin OpenGL drawing
  for (int i = 0; i < pawnModel.num_faces; i++)
    {
      glTexCoord2f (pawnModel.uvs[pawnModel.v_idx[i]].x, pawnModel.uvs[pawnModel.v_idx[i]].y);

      glNormal3f (pawnModel.normals[pawnModel.n_idx[i]].x,
                  pawnModel.normals[pawnModel.n_idx[i]].y,
                  pawnModel.normals[pawnModel.n_idx[i]].z);
      glVertex3f (pawnModel.vertices[pawnModel.v_idx[i]].x,
                  pawnModel.vertices[pawnModel.v_idx[i]].y,
                  pawnModel.vertices[pawnModel.v_idx[i]].z);
    }
  glEndList (); // Done with display list

  return listID;
}

pawn::~pawn ()
{
  glDeleteLists (callId, 1);
}

