#include <iostream>
#include "models/knightModel.h"
#include "knight.h"
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

Obj knightModel[4];

knight::knight ()
{
  knightModel[0].vertices = Cylinder24_vertex;
  knightModel[0].v_idx = Cylinder24_vidx;
  knightModel[0].normals = Cylinder24_normal;
  knightModel[0].n_idx = Cylinder24_nidx;
  knightModel[0].uvs = Cylinder24_uv;
  knightModel[0].num_faces = (sizeof (Cylinder24_vidx) / sizeof (long));

  knightModel[1].vertices = Cylinder28_vertex;
  knightModel[1].v_idx = Cylinder28_vidx;
  knightModel[1].normals = Cylinder28_normal;
  knightModel[1].n_idx = Cylinder28_nidx;
  knightModel[1].uvs = Cylinder28_uv;
  knightModel[1].num_faces = (sizeof (Cylinder28_vidx) / sizeof (long));

  knightModel[2].vertices = Sphere14_vertex;
  knightModel[2].v_idx = Sphere14_vidx;
  knightModel[2].normals = Sphere14_normal;
  knightModel[2].n_idx = Sphere14_nidx;
  knightModel[2].uvs = Sphere14_uv;
  knightModel[2].num_faces = (sizeof (Sphere14_vidx) / sizeof (long));

  knightModel[3].vertices = Sphere17_vertex;
  knightModel[3].v_idx = Sphere17_vidx;
  knightModel[3].normals = Sphere17_normal;
  knightModel[3].n_idx = Sphere17_nidx;
  knightModel[3].uvs = Sphere17_uv;
  knightModel[3].num_faces = (sizeof (Sphere17_vidx) / sizeof (long));

  callId = createCallList ();
}

void
knight::drawUnit (int location)
{
  glBegin (GL_TRIANGLES);
  glCallList (callId);
  glEnd ();
}

vector<int>
knight::moves (int location, const vector< vector <board::placement> > &boardState)
{
  return knightMoves (location, boardState);
}

GLuint
knight::createCallList ()
{
  GLuint listID = glGenLists (1);
  glNewList (listID, GL_COMPILE); // Begin OpenGL drawing
  for (int z = 0; z < 4; z++)
    {
      for (int i = 0; i < knightModel[z].num_faces; i++)
        {
          glTexCoord2f (knightModel[z].uvs[knightModel[z].v_idx[i]].x, knightModel[z].uvs[knightModel[z].v_idx[i]].y);

          glNormal3f (knightModel[z].normals[knightModel[z].n_idx[i]].x,
                      knightModel[z].normals[knightModel[z].n_idx[i]].y,
                      knightModel[z].normals[knightModel[z].n_idx[i]].z);
          glVertex3f (knightModel[z].vertices[knightModel[z].v_idx[i]].x,
                      knightModel[z].vertices[knightModel[z].v_idx[i]].y,
                      knightModel[z].vertices[knightModel[z].v_idx[i]].z);
        }
    }
  glEndList (); // Done with display list

  return listID;
}

knight::~knight ()
{
  glDeleteLists (callId, 4);
}

