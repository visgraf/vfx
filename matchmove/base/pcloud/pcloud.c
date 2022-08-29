#include "pcloud.h"


void pcloud_init( PointCloud *pc, int frame, int npoints )
{
 pc->m = gsl_matrix_alloc( npoints, 2 );
 pc->frame = frame;
}


void pcloud_free( PointCloud *pc )
{
 gsl_matrix_free( pc->m );
 free( pc );
}


void pcloud_get_point( gsl_vector *v, PointCloud *pc, int i )
{
  gsl_matrix_get_row( v, pc->m, i );
}


void pcloud_set_point( PointCloud *pc, int i,  gsl_vector *v )
{
  gsl_matrix_set_row( pc->m, i, v );
}


int pcloud_get_npoints( PointCloud *pc )
{
 return pc->m->size1;
}


void pcloud_read( PointCloud *pc, FILE *f )
{
 double val;
 int i, j, frame, npoints;
 fscanf( f, "Frame %i\n", &frame );
 fscanf( f, "Npoints %i\n", &npoints );
 pcloud_init( pc, frame, npoints );
 
 for( i = 0; i < npoints; i++ )
    for( j = 0; j < 2; j++ ){
      fscanf( f, "%lf", &val );
      gsl_matrix_set( pc->m, i, j, val ); 
    } 
 fscanf( f, "\n" );
}



