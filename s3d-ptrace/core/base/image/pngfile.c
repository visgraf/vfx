/*
 * Copyright 2002-2011 Guillaume Cottenceau and contributors.
 *
 */

#include "laux.h"
#include "pngfile.h"

#include "png.h"

#undef PNG_LIBPNG_VER_STRING 
#define PNG_LIBPNG_VER_STRING "1.5.13"


unsigned char *read_png_file(FILE *fp, int *cols, int *rows)
{
  int y, width, height, rowbytes;
  png_byte color_type;
  png_byte bit_depth;
  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep * row_pointers;
  png_byte * buf_pointer;
  unsigned char header[8];

  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8))
    error("[read_png_file] File is not recognized as a PNG file");
  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    error("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    error("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    error("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  if (bit_depth != 8 || color_type != 2)
    error("[read_png_file] bit depth ne 8 | color ne 2");

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    error("[read_png_file] Error during read_image");

  row_pointers = (png_bytep*) emalloc(sizeof(png_bytep) * height);
  buf_pointer = (png_byte*) emalloc(sizeof(png_byte) * width * height * 3);
  rowbytes = width*3;

  for (y=0; y<height; y++)
    row_pointers[y] = buf_pointer + (y * rowbytes);

  png_read_image(png_ptr, row_pointers);

  /* Clean up after the read, and efree any memory allocated - REQUIRED */
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  efree(row_pointers);

  *cols = width;
  *rows = height;
  return (unsigned char *) buf_pointer;
}


void write_png_file(FILE *fp, int width, int height, unsigned char *pixels)
{
  int y, rowbytes;
  png_structp png_ptr;
  png_infop info_ptr;
  png_byte color_type = 2;
  png_byte bit_depth = 8;
  png_bytep * row_pointers;
  png_byte * buf_pointer;

  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    error("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    error("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    error("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    error("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height,
	       bit_depth, color_type, PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    error("[write_png_file] Error during writing bytes");

  row_pointers = (png_bytep*) emalloc(sizeof(png_bytep) * height);
  rowbytes = width*3;
  buf_pointer = (png_byte *) pixels;

  for (y=0; y<height; y++)
    row_pointers[y] = buf_pointer + (y * rowbytes);

  png_write_image(png_ptr, row_pointers);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    error("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  efree(row_pointers);
}

