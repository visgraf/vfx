#ifndef HDR_H
#define HDR_H

#include "color.h"
#include "image.h"
#include "string.h"


typedef struct HDRImage{
  int w, h;
  Color *c;
}HDRImage;


HDRImage *hdrimg_init( int w, int h);
void hdrimg_free( HDRImage *img );
void hdrimg_write(HDRImage *i, char *fname);
HDRImage *hdrimg_read(char *fname);

Color hdrimg_getc( HDRImage *p, int u, int v );
void hdrimg_putc( HDRImage *p, int u, int v, Color c );


#endif
