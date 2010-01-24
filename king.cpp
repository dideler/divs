#include <iostream>
#include "models/kingModel.h"
#include "king.h"
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

Obj kingModel[7];

king::king ()
{
  kingModel[0].vertices = Cylinder19_vertex;
  kingModel[0].v_idx = Cylinder19_vidx;
  kingModel[0].normals = Cylinder19_normal;
  kingModel[0].n_idx = Cylinder19_nidx;
  kingModel[0].uvs = Cylinder19_uv;
  kingModel[0].num_faces = (sizeof (Cylinder19_vidx) / sizeof (long));

  kingModel[1].vertices = Cylinder20_vertex;
  kingModel[1].v_idx = Cylinder20_vidx;
  kingModel[1].normals = Cylinder20_normal;
  kingModel[1].n_idx = Cylinder20_nidx;
  kingModel[1].uvs = Cylinder20_uv;
  kingModel[1].num_faces = (sizeof (Cylinder20_vidx) / sizeof (long));

  kingModel[2].vertices = Cylinder21_vertex;
  kingModel[2].v_idx = Cylinder21_vidx;
  kingModel[2].normals = Cylinder21_normal;
  kingModel[2].n_idx = Cylinder21_nidx;
  kingModel[2].uvs = Cylinder21_uv;
  kingModel[2].num_faces = (sizeof (Cylinder21_vidx) / sizeof (long));

  kingModel[3].vertices = Box02_vertex;
  kingModel[3].v_idx = Box02_vidx;
  kingModel[3].normals = Box02_normal;
  kingModel[3].n_idx = Box02_nidx;
  kingModel[3].uvs = Box02_uv;
  kingModel[3].num_faces = (sizeof (Box02_vidx) / sizeof (long));

  kingModel[4].vertices = Duplicate00_vertex;
  kingModel[4].v_idx = Duplicate00_vidx;
  kingModel[4].normals = Duplicate00_normal;
  kingModel[4].n_idx = Duplicate00_nidx;
  kingModel[4].uvs = Duplicate00_uv;
  kingModel[4].num_faces = (sizeof (Duplicate00_vidx) / sizeof (long));

  kingModel[5].vertices = Duplicate01_vertex;
  kingModel[5].v_idx = Duplicate01_vidx;
  kingModel[5].normals = Duplicate01_normal;
  kingModel[5].n_idx = Duplicate01_nidx;
  kingModel[5].uvs = Duplicate01_uv;
  kingModel[5].num_faces = (sizeof (Duplicate01_vidx) / sizeof (long));

  kingModel[6].vertices = Box03_vertex;
  kingModel[6].v_idx = Box03_vidx;
  kingModel[6].normals = Box03_normal;
  kingModel[6].n_idx = Box03_nidx;
  kingModel[6].uvs = Box03_uv;
  kingModel[6].num_faces = (sizeof (Box03_vidx) / sizeof (long));

  callId = createCallList ();
}

void
king::drawUnit (int location)
{
  glBegin (GL_TRIANGLES);
  glCallList (callId);
  glEnd ();
}

vector<int>
king::moves (int location, const vector< vector <board::placement> > &boardState)
{
  return kingMoves (location, boardState);
}

GLuint
king::createCallList ()
{
  GLuint listID = glGenLists (1);
  glNewList (listID, GL_COMPILE); // Begin OpenGL drawing
  for (int z = 0; z < 7; z++)
    {
      for (int i = 0; i < kingModel[z].num_faces; i++)
        {
          glTexCoord2f (kingModel[z].uvs[kingModel[z].v_idx[i]].x, kingModel[z].uvs[kingModel[z].v_idx[i]].y);

          glNormal3f (kingModel[z].normals[kingModel[z].n_idx[i]].x,
                      kingModel[z].normals[kingModel[z].n_idx[i]].y,
                      kingModel[z].normals[kingModel[z].n_idx[i]].z);
          glVertex3f (kingModel[z].vertices[kingModel[z].v_idx[i]].x,
                      kingModel[z].vertices[kingModel[z].v_idx[i]].y,
                      kingModel[z].vertices[kingModel[z].v_idx[i]].z);
        }
    }
  glEndList (); // Done with display list

  return listID;
}

king::~king ()
{
  glDeleteLists (callId, 1);
}

