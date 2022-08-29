#include "calib.h"

void calib_rt_transform( gsl_vector *xcam, gsl_matrix *r, gsl_vector *t, gsl_vector *x )
{
 gsl_matrix *aux1, *aux2;

 aux1 = gsl_matrix_alloc(3, 1);
 aux2 = gsl_matrix_alloc(3, 1);
 gsl_matrix_set_col(aux1, 0, x);
 gsl_linalg_matmult(r, aux1, aux2);
 gsl_matrix_set_col(aux1, 0, t);
 gsl_matrix_add(aux1, aux2);
 gsl_matrix_get_col(xcam, aux1, 0);

 gsl_matrix_free(aux1);
 gsl_matrix_free(aux2);
}

