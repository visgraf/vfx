#include "cameras.h"
 
 
Cameras *cameras_alloc( gsl_matrix *k, gsl_matrix *r[MAX_CAMERAS], 
                        gsl_vector *t[MAX_CAMERAS], int ncams )
{
 int i;
 Cameras *c = NEWSTRUCT(Cameras);
 
 c->k = k;
 for( i = 0; i < ncams; i++ ){
    c->r[i] = r[i];
    c->t[i] = t[i];
 }
 c->ncams = ncams;
 
 return c;
}


Cameras *cameras_read( gsl_matrix *k, FILE *f )
{
 int i = 0; 
 gsl_matrix *r[MAX_CAMERAS];
 gsl_vector *t[MAX_CAMERAS];
 
 while( !feof(f) ){
   fscanf( f, "Frame %i\n", &i );
   r[i] = gsl_matrix_alloc(3,3);
   gsl_matrix_fscanf( f, r[i] ); 
   fscanf( f, "\n");
   t[i] = gsl_vector_alloc(3);
   gsl_vector_fscanf( f, t[i] ); 
   fscanf( f, "\n");
   i++;
 }
 
 return cameras_alloc( k, r, t, i );
}


void cameras_free( Cameras *c )
{
 int i;
 
 gsl_matrix_free(c->k);
 for( i = 0; i < c->ncams; i++ ){
   gsl_matrix_free( c->r[i] );
   gsl_vector_free( c->t[i] ); 
 } 
}

                                  
void cameras_apply( Cameras *c, gsl_vector *p, gsl_vector *proj, int i )  
{
 gsl_matrix *m;
 
 m = gsl_matrix_alloc( 3, 4 ); 
 calib_pmatrix_make( m, c->k, c->r[i], c->t[i] );
 calib_apply_P( m, p, proj );
 
 gsl_matrix_free(m);
}   


void cameras_getP( gsl_matrix *p, Cameras *c, int i )
{
 calib_pmatrix_make( p, c->k, c->r[i], c->t[i] );
} 

