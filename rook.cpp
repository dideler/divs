#include <iostream>
#include "models/rookModel.h"
#include "rook.h"
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

Obj rookModel[6];

rook::rook ()
{
  rookModel[0].vertices = Cylinder077_vertex;
  rookModel[0].v_idx = Cylinder077_vidx;
  rookModel[0].normals = Cylinder077_normal;
  rookModel[0].n_idx = Cylinder077_nidx;
  rookModel[0].uvs = Cylinder077_uv;
  rookModel[0].num_faces = (sizeof (Cylinder077_vidx) / sizeof (long));

  rookModel[1].vertices = Cylinder09_vertex;
  rookModel[1].v_idx = Cylinder09_vidx;
  rookModel[1].normals = Cylinder09_normal;
  rookModel[1].n_idx = Cylinder09_nidx;
  rookModel[1].uvs = Cylinder09_uv;
  rookModel[1].num_faces = (sizeof (Cylinder09_vidx) / sizeof (long));

  rookModel[2].vertices = Cylinder111_vertex;
  rookModel[2].v_idx = Cylinder111_vidx;
  rookModel[2].normals = Cylinder111_normal;
  rookModel[2].n_idx = Cylinder111_nidx;
  rookModel[2].uvs = Cylinder111_uv;
  rookModel[2].num_faces = (sizeof (Cylinder111_vidx) / sizeof (long));

  rookModel[3].vertices = Cylinder12_vertex;
  rookModel[3].v_idx = Cylinder12_vidx;
  rookModel[3].normals = Cylinder12_normal;
  rookModel[3].n_idx = Cylinder12_nidx;
  rookModel[3].uvs = Cylinder12_uv;
  rookModel[3].num_faces = (sizeof (Cylinder12_vidx) / sizeof (long));

  rookModel[4].vertices = Cylinder13_vertex;
  rookModel[4].v_idx = Cylinder13_vidx;
  rookModel[4].normals = Cylinder13_normal;
  rookModel[4].n_idx = Cylinder13_nidx;
  rookModel[4].uvs = Cylinder13_uv;
  rookModel[4].num_faces = (sizeof (Cylinder13_vidx) / sizeof (long));

  rookModel[5].vertices = Cylinder14_vertex;
  rookModel[5].v_idx = Cylinder14_vidx;
  rookModel[5].normals = Cylinder14_normal;
  rookModel[5].n_idx = Cylinder14_nidx;
  rookModel[5].uvs = Cylinder14_uv;
  rookModel[5].num_faces = (sizeof (Cylinder14_vidx) / sizeof (long));

  callId = createCallList ();
}

void
rook::drawUnit (int location)
{
  glBegin (GL_TRIANGLES);
  glCallList (callId);
  glEnd ();
}

vector<int>
rook::moves (int location, const vector< vector <board::placement> > &boardState)
{
  return rookMoves (location, boardState);
}

GLuint
rook::createCallList ()
{
  GLuint listID = glGenLists (1);
  glNewList (listID, GL_COMPILE); // Begin OpenGL drawing
  for (int z = 0; z < 6; z++)
    {
      for (int i = 0; i < rookModel[z].num_faces; i++)
        {
          glTexCoord2f (rookModel[z].uvs[rookModel[z].v_idx[i]].x, rookModel[z].uvs[rookModel[z].v_idx[i]].y);

          glNormal3f (rookModel[z].normals[rookModel[z].n_idx[i]].x,
                      rookModel[z].normals[rookModel[z].n_idx[i]].y,
                      rookModel[z].normals[rookModel[z].n_idx[i]].z);
          glVertex3f (rookModel[z].vertices[rookModel[z].v_idx[i]].x,
                      rookModel[z].vertices[rookModel[z].v_idx[i]].y,
                      rookModel[z].vertices[rookModel[z].v_idx[i]].z);
        }
    }
  glEndList (); // Done with display list

  return listID;
}

rook::~rook ()
{
  glDeleteLists (callId, 1);
}

