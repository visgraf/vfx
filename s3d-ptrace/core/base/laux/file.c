#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "defs.h"

static char *dpath = NULL;

/*:
** 	set directory path
 */
void dsetpath(char *pathname)
{
  if (dpath != NULL)
    efree(dpath);
  asprintf(&dpath, "%s", pathname);
}

/*:
** 	open file in directory
 */
FILE *dfopen(char *fname, char *mode)
{
  FILE *fd;
  char fullpath[1024];
  if (dpath != NULL && fname[0] != '/')
    snprintf(fullpath, 1024, "%s/%s", dpath, fname);
  else
    snprintf(fullpath, 1024, "%s", fname);
  fd = fopen(fullpath, mode);
  if (fd == NULL)
    error("(dfopen) opening file");
  return fd;
}

