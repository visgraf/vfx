#include "calib.h"

void calib_ematrix_get_P( gsl_matrix *k, gsl_matrix *e, gsl_vector *x1,
                          gsl_vector *x2, gsl_matrix *p )
{
 gsl_vector *t;
 gsl_matrix *r;

 t = gsl_vector_alloc(3);
 r = gsl_matrix_alloc(3,3);

 calib_ematrix_get_RT( k, e, x1, x2, r, t );
 calib_pmatrix_make( p, k, r, t );

 gsl_matrix_free(r);
 gsl_vector_free(t);
}
