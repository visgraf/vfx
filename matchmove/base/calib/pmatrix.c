#include "calib.h"

void calib_pmatrix_make( gsl_matrix *p, gsl_matrix *k, gsl_matrix *r, gsl_vector *t )
{
 int i;
 gsl_vector *v;
 gsl_matrix *paux;

 v = gsl_vector_alloc(3);
 paux = gsl_matrix_alloc(3,4);

 for( i=0; i<3; i++ ){
   gsl_matrix_get_col( v, r, i );
   gsl_matrix_set_col( paux, i, v );
 }

 gsl_matrix_set_col( paux, 3, t );
 gsl_linalg_matmult( k, paux, p );
 gsl_vector_free(v);
 gsl_matrix_free(paux);
}
