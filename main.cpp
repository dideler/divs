#include <cstdlib>
#include "graphics.h"

int
main (int argc, char** argv)
{
  graphics* gl = new graphics (argc, argv);
  delete gl;
  return (EXIT_SUCCESS);
}

