/*  pngfile.h - read png */

#ifndef PNGFILE_H
#define PNGFILE_H

#include <stdio.h>

unsigned char *read_png_file(FILE *fp, int *cols, int *rows);
void write_png_file(FILE *fp, int width, int height, unsigned char *pixels);

#endif
