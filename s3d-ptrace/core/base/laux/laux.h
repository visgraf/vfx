#ifndef AUXL_H
#define AUXL_H 1

#include <stdio.h>

/* auxiliary functions */

int error(const char *message);
void warning(const char *message);

void *emalloc(int siz);
void *erealloc(void *buf, int siz);
void efree(void *buf);

int ecreat(char *file, int perms);
int eopen(char *file, int mode);

int eread(int fd, void *ptr, int numb);
int ewrite(int fd, void *ptr, int numb);

void dsetpath(char *pathname);
FILE *dfopen(char *fname, char *mode);

#endif
