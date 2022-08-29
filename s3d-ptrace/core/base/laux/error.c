#include "defs.h"

int error(const char *message)
{
  fprintf(stderr, "fatal error: %s\n", message);
  exit(0);
  return -1;
}


void warning(const char *message)
{
  fprintf(stderr, "warning: %s\n", message);
}
