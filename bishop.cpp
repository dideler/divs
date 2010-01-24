#include <iostream>
#include "models/bishopModel.h"
#include "bishop.h"
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
}; ///< structure used by the model

Obj bishopModel[3];

bishop::bishop ()
{

  // import models
  bishopModel[0].vertices = Cylinder02_vertex;
  bishopModel[0].v_idx = Cylinder02_vidx;
  bishopModel[0].normals = Cylinder02_normal;
  bishopModel[0].n_idx = Cylinder02_nidx;
  bishopModel[0].uvs = Cylinder02_uv;
  bishopModel[0].num_faces = (sizeof (Cylinder02_vidx) / sizeof (long));

  bishopModel[1].vertices = GeoSphere01_vertex;
  bishopModel[1].v_idx = GeoSphere01_vidx;
  bishopModel[1].normals = GeoSphere01_normal;
  bishopModel[1].n_idx = GeoSphere01_nidx;
  bishopModel[1].uvs = GeoSphere01_uv;
  bishopModel[1].num_faces = (sizeof (GeoSphere01_vidx) / sizeof (long));

  bishopModel[2].vertices = GeoSphere02_vertex;
  bishopModel[2].v_idx = GeoSphere02_vidx;
  bishopModel[2].normals = GeoSphere02_normal;
  bishopModel[2].n_idx = GeoSphere02_nidx;
  bishopModel[2].uvs = GeoSphere02_uv;
  bishopModel[2].num_faces = (sizeof (GeoSphere02_vidx) / sizeof (long));

  // generate call list
  callId = createCallList ();
}

void
bishop::drawUnit (int location)
{

  glBegin (GL_TRIANGLES);
  glCallList (callId);
  glEnd ();

}

vector<int>
bishop::moves (int location, const vector< vector <board::placement> > &boardState)
{
  return bishopMoves (location, boardState);
}

GLuint
bishop::createCallList ()
{
  GLuint listID = glGenLists (1);
  glNewList (listID, GL_COMPILE); // Begin OpenGL drawing
  for (int z = 0; z < 3; z++)
    {
      for (int i = 0; i < bishopModel[z].num_faces; i++)
        {
          glTexCoord2f (bishopModel[z].uvs[bishopModel[z].v_idx[i]].x, bishopModel[z].uvs[bishopModel[z].v_idx[i]].y);

          glNormal3f (bishopModel[z].normals[bishopModel[z].n_idx[i]].x,
                      bishopModel[z].normals[bishopModel[z].n_idx[i]].y,
                      bishopModel[z].normals[bishopModel[z].n_idx[i]].z);
          glVertex3f (bishopModel[z].vertices[bishopModel[z].v_idx[i]].x,
                      bishopModel[z].vertices[bishopModel[z].v_idx[i]].y,
                      bishopModel[z].vertices[bishopModel[z].v_idx[i]].z);
        }
    }
  glEndList (); // Done with display list

  return listID;
}

bishop::~bishop ()
{
  glDeleteLists (callId, 1);
}

