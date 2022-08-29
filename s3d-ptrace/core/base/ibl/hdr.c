#include "hdr.h"


static void read_line( FILE *f, char *s );


HDRImage *hdrimg_init( int w, int h )
{
 HDRImage *img = NEWSTRUCT(HDRImage);
 
 img->w = w;
 img->h = h;
 img->c = NEWTARRAY( w*h, Color );
 return img;
}


void hdrimg_free( HDRImage *img )
{
 free( img->c );
 free( img );
}


Color hdrimg_getc( HDRImage *img, int x, int y )
{
 return img->c[img->w*y + x];
}


void hdrimg_putc( HDRImage *img, int x, int y, Color c )
{
 img->c[img->w*y + x] = c;
}


void hdrimg_write(HDRImage *img, char *fname)
{
 Color c;
 float cx, cy, cz;
 char s[10];
 int x, y, xres, yres;
 FILE *f = fopen( fname, "wb" );
 
 fwrite( "PF\n", 3, 1, f );
 sprintf( s, "%i %i\n", img->w, img->h );
 fwrite( s, strlen(s), 1, f );
 sprintf( s, "-1.0\n" );
 fwrite( s, strlen(s), 1, f );
 for ( y = 0; y < img->h; y++ )
   for ( x = 0; x < img->w; x++ ){
     c = hdrimg_getc( img, x, y );
     cx = (float)c.x;
     fwrite( &cx, sizeof(float), 1, f );
     cy = (float)c.y;
     fwrite( &cy, sizeof(float), 1, f );
     cz = (float)c.z;
     fwrite( &cz, sizeof(float), 1, f ); 
   }
 fclose(f);
}


HDRImage *hdrimg_read(char *fname)
{
 HDRImage *img;
 int x, y, xres, yres;
 float r, g, b, v;
 char s[100];
 FILE *f = fopen( fname, "rb" );
 
 read_line( f, s );
 if( strcmp( s, "PF\n" ) )
   error("wrong PFM File Format\n" ); 
 read_line( f, s );
 sscanf( s, "%d %d\n", &xres, &yres );
 read_line( f, s );
 sscanf( s, "%f", &v );
 if( v != -1 ) 
   error("wrong PFM File Format\n" );
 img = hdrimg_init( xres, yres );
  
 if( v == -1 ){
   for( y = 0; y < img->h; y++ )
     for( x = 0; x < img->w; x++ ){
       fread( &r, sizeof(float), 1, f );
       fread( &g, sizeof(float), 1, f );
       fread( &b, sizeof(float), 1, f );
       hdrimg_putc( img, x, y, c_make(r, g, b) );
     }
 }
 fclose(f);   
 return img;
}


void read_line( FILE *f, char *s )
{
 int i = 0;
 do{
   fread( &s[i], 1, 1, f );
 }  
 while( s[i++] != 0x0A );
 s[i] = 0x00;
}

