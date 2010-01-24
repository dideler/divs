#include <iostream>
#include "models/queenModel.h"
#include "queen.h"
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

Obj queenModel[7];

queen::queen ()
{
  queenModel[0].vertices = Cylinder04_vertex;
  queenModel[0].v_idx = Cylinder04_vidx;
  queenModel[0].normals = Cylinder04_normal;
  queenModel[0].n_idx = Cylinder04_nidx;
  queenModel[0].uvs = Cylinder04_uv;
  queenModel[0].num_faces = (sizeof (Cylinder04_vidx) / sizeof (long));

  queenModel[1].vertices = Cylinder07_vertex;
  queenModel[1].v_idx = Cylinder07_vidx;
  queenModel[1].normals = Cylinder07_normal;
  queenModel[1].n_idx = Cylinder07_nidx;
  queenModel[1].uvs = Cylinder07_uv;
  queenModel[1].num_faces = (sizeof (Cylinder07_vidx) / sizeof (long));

  queenModel[2].vertices = Cylinder11_vertex;
  queenModel[2].v_idx = Cylinder11_vidx;
  queenModel[2].normals = Cylinder11_normal;
  queenModel[2].n_idx = Cylinder11_nidx;
  queenModel[2].uvs = Cylinder11_uv;
  queenModel[2].num_faces = (sizeof (Cylinder11_vidx) / sizeof (long));

  queenModel[3].vertices = Sphere01_vertex;
  queenModel[3].v_idx = Sphere01_vidx;
  queenModel[3].normals = Sphere01_normal;
  queenModel[3].n_idx = Sphere01_nidx;
  queenModel[3].uvs = Sphere01_uv;
  queenModel[3].num_faces = (sizeof (Sphere01_vidx) / sizeof (long));

  queenModel[4].vertices = Sphere02_vertex;
  queenModel[4].v_idx = Sphere02_vidx;
  queenModel[4].normals = Sphere02_normal;
  queenModel[4].n_idx = Sphere02_nidx;
  queenModel[4].uvs = Sphere02_uv;
  queenModel[4].num_faces = (sizeof (Sphere02_vidx) / sizeof (long));

  queenModel[5].vertices = Sphere03_vertex;
  queenModel[5].v_idx = Sphere03_vidx;
  queenModel[5].normals = Sphere03_normal;
  queenModel[5].n_idx = Sphere03_nidx;
  queenModel[5].uvs = Sphere03_uv;
  queenModel[5].num_faces = (sizeof (Sphere03_vidx) / sizeof (long));

  queenModel[6].vertices = Cylinder17_vertex;
  queenModel[6].v_idx = Cylinder17_vidx;
  queenModel[6].normals = Cylinder17_normal;
  queenModel[6].n_idx = Cylinder17_nidx;
  queenModel[6].uvs = Cylinder17_uv;
  queenModel[6].num_faces = (sizeof (Cylinder17_vidx) / sizeof (long));

  callId = createCallList ();
}

void
queen::drawUnit (int location)
{
  glBegin (GL_TRIANGLES);
  glCallList (callId);
  glEnd ();
}

vector<int>
queen::moves (int location, const vector< vector <board::placement> > &boardState)
{
  return queenMoves (location, boardState);
}

GLuint
queen::createCallList ()
{
  GLuint listID = glGenLists (1);
  glNewList (listID, GL_COMPILE); // Begin OpenGL drawing
  for (int z = 0; z < 7; z++)
    {
      for (int i = 0; i < queenModel[z].num_faces; i++)
        {
          glTexCoord2f (queenModel[z].uvs[queenModel[z].v_idx[i]].x, queenModel[z].uvs[queenModel[z].v_idx[i]].y);

          glNormal3f (queenModel[z].normals[queenModel[z].n_idx[i]].x,
                      queenModel[z].normals[queenModel[z].n_idx[i]].y,
                      queenModel[z].normals[queenModel[z].n_idx[i]].z);
          glVertex3f (queenModel[z].vertices[queenModel[z].v_idx[i]].x,
                      queenModel[z].vertices[queenModel[z].v_idx[i]].y,
                      queenModel[z].vertices[queenModel[z].v_idx[i]].z);
        }
    }
  glEndList (); // Done with display list

  return listID;
}

queen::~queen ()
{
  glDeleteLists (callId, 1);
}

