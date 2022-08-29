#include "singlecam.h"

static int get_nlines( FILE *f );

int main( int argc, char **argv )
{
 int npoints;
 FILE *fprojs, *fpoints;
 FILE *fk, *fr, *ft;
 gsl_matrix *projs, *points, *p;
 gsl_vector *x, *xout, *r_vec, *t;
 gsl_matrix *k, *r;
 
 k = gsl_matrix_alloc( 3, 3 );
 r = gsl_matrix_alloc( 3, 3 );
 r_vec = gsl_vector_alloc( 3 );
 t = gsl_vector_alloc( 3 );
 p = gsl_matrix_alloc( 3, 4 );
 fprojs = fopen( argv[1], "r" );
 npoints = get_nlines( fprojs );
 fclose( fprojs );
 projs = gsl_matrix_alloc( npoints, 2 );
 points = gsl_matrix_alloc( npoints, 3 );
 x = singlecam_param_alloc();
 xout =  singlecam_param_alloc();

 fprojs = fopen( argv[1], "r" );
 fpoints = fopen( argv[2], "r" );
 gsl_matrix_fscanf( fprojs, projs );
 gsl_matrix_fscanf( fpoints, points );  
 fclose( fprojs );
 fclose( fpoints );
 
 calib_singlecam_dlt( p, points, projs );
 calib_camfactor( k, r, t, p );
  
 fk = fopen( "k", "w" );
 fr = fopen( "r", "w" );
 ft = fopen( "t", "w" );
 gsl_matrix_fprintf( fk, k, "%f" );
 gsl_matrix_fprintf( fr, r, "%f" );
 gsl_vector_fprintf( ft, t, "%f" );
 fclose(fk);
 fclose(fr);
 fclose(ft);
 
 gsl_matrix_free(p);
 gsl_matrix_free(projs);
 gsl_matrix_free(points);
 gsl_vector_free(x);
 gsl_vector_free(xout);
 gsl_matrix_free(k);
 gsl_matrix_free(r);
 gsl_vector_free(r_vec);
 gsl_vector_free(t);
}


int get_nlines( FILE *f )
{
 char c;
 int nlines = 0;
 double x1, y1;

 while( !feof( f ) ){
   if( (c = getc(f)) != '%' ){
     ungetc( c, f );
     fscanf( f, "%lf %lf\n", &x1, &y1);
     nlines++;
   }
 }
 return nlines;
}
