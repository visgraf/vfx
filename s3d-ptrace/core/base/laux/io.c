#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "defs.h"

/*:
** 	creates a file descriptor with error message in failure
 */
int ecreat(char *file, int perms)
{
  int fd;
  if((fd = creat(file, perms)) != -1)
    return(fd);
  else
    return error("can't create file");
}


/*:
** 	returns a file descriptor to a file with  error message
 */
int eopen(char *file, int mode)
{
  int fd;
  if((fd=open(file, mode)) != -1)
    return(fd);
  else
    return error("can't open file");
}

/*:
** 	reads a file descriptor with error message in failure
 */
int eread(int fd, void *ptr, int numb)
{
  int n;
  if((n=read(fd, ptr, numb)) == -1)
    return error("can't read from file");
  else
    return n;
}

/*:
** 	writes a file descriptor with error message in failure
 */
int ewrite(int fd, void *ptr, int numb)
{
  int n;
  if((n = write(fd, ptr, numb)) == -1)
    return error("can't write to file");
  else
    return n;
}

