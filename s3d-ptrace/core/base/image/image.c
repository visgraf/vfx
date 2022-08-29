#ifdef WIN32
#define CYGWIN
#endif

#ifdef CYGWIN
#include <fcntl.h>
#endif

#include "image.h"
#include "pngfile.h"

#include <string.h>

#define PIX_MIN 0
#define PIX_MAX 255

#define PIXRED(I,U,V) I->c[U + (((I->h - 1) - (V)) * I->w)].x
#define PIXGRN(I,U,V) I->c[U + (((I->h - 1) - (V)) * I->w)].y
#define PIXBLU(I,U,V) I->c[U + (((I->h - 1) - (V)) * I->w)].z


Image *img_init(int type, int w, int h)
{
  Image *i = NEWSTRUCT(Image);
  
  i->w = w; i->h = h;
  i->c = NEWTARRAY(w*h, Bcolor);
  img_clear(i, C_BLACK);
  return i;
}


void img_clear(Image *i, Color c)
{
  int u,v;
  for (v = 0; v < i->h; v++)
    for (u = 0; u < i->w; u++)
      img_putc(i,u,v,c);
}


Image *img_read(char *fname)
{
  FILE *fp;
  int cols, rows;
  Byte *pixels;
  Image *i;
  
  fp =  (strcmp("stdin", fname) == 0)? stdin : dfopen(fname, "rb");
  if (fp == NULL) {
    warning("image read: could not open file");
    return img_init(IMG_TRUECOL, 256,256);
  }
  pixels = read_png_file(fp, &cols, &rows);
  i = img_init(0, cols, rows);
  memcpy(i->c, pixels, sizeof(Byte)*cols*rows*3);
  efree(pixels);
  fclose(fp);
  return i;
}


void img_write(Image *i, char *fname, int cflag)
{
  FILE *fp;

  fp = (strcmp("stdout", fname) == 0)? stdout : dfopen(fname, "wb");
  write_png_file(fp,  i->w, i->h, (unsigned char *)(i->c));
  if  (strcmp("stdout", fname) != 0) 
    fclose(fp);
}


void img_putc(Image *i, int u, int v, Color c)
{
  if (u >= 0 && u < i->w && v >= 0 && v < i->h) {
    PIXRED(i,u,v) = CLAMP(RED(c), PIX_MIN, PIX_MAX); 
    PIXGRN(i,u,v) = CLAMP(GRN(c), PIX_MIN, PIX_MAX); 
    PIXBLU(i,u,v) = CLAMP(BLU(c), PIX_MIN, PIX_MAX);
  }
}

Color img_getc(Image *i, int u, int v)
{
  if (u >= 0 && u < i->w && v >= 0 && v < i->h)
    return c_make(PIXRED(i,u,v),PIXGRN(i,u,v),PIXBLU(i,u,v));
  else
    return C_BLACK;
}

void img_free(Image *i)
{
  efree(i->c); efree(i);
}
