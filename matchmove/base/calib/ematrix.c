#include "calib.h"

int is_rotation( gsl_matrix *r );


void calib_ematrix_make( gsl_matrix *e, gsl_matrix *f, gsl_matrix *k )
{
 gsl_matrix *aux, *kaux;

 aux = gsl_matrix_alloc( 3, 3 );
 kaux = gsl_matrix_alloc( 3, 3 );

 gsl_linalg_matmult( f, k, aux );
 gsl_matrix_memcpy( kaux, k );
 gsl_matrix_transpose( kaux );
 gsl_linalg_matmult( kaux, aux, e );

 gsl_matrix_free( aux );
 gsl_matrix_free( kaux );
}


void calib_ematrix_get_all_rt( gsl_matrix *q, gsl_matrix *r1, gsl_matrix *r2, gsl_vector *t )
{
 gsl_matrix *e, *v, *aux, *qaux;
 gsl_vector *s, *w;

 s = gsl_vector_alloc( 3 );
 w = gsl_vector_alloc( 3 );
 v = gsl_matrix_alloc( 3, 3 );
 e = gsl_matrix_alloc( 3, 3 );
 aux = gsl_matrix_alloc( 3, 3 );
 qaux = gsl_matrix_alloc( 3, 3 );
 gsl_matrix_memcpy( qaux, q );

 /* creating matrix W */
 gsl_matrix_set_zero(e);
 gsl_matrix_set( e, 0, 1,  1. );
 gsl_matrix_set( e, 1, 0, -1. );
 gsl_matrix_set( e, 2, 2,  1. );

 gsl_linalg_SV_decomp( qaux, v, s, w );
 gsl_linalg_matmult( qaux, e, aux );
 gsl_matrix_transpose( v );
 gsl_linalg_matmult( aux, v, r1 );
 if( !is_rotation(r1) )
   gsl_matrix_scale( r1, -1. );
 gsl_matrix_get_col( t, qaux, 2 );

 gsl_matrix_transpose(e);
 gsl_linalg_matmult( qaux, e, aux );
 gsl_linalg_matmult( aux, v, r2 );
 if( !is_rotation(r2) )
   gsl_matrix_scale( r2, -1. );

 gsl_vector_free(s);
 gsl_vector_free(w);
 gsl_matrix_free(v);
 gsl_matrix_free(e);
 gsl_matrix_free(aux);
 gsl_matrix_free(qaux);
}


/* Return 1 if <T(i)XT(j), T(K)> > 0 */

int is_rotation( gsl_matrix *r )
{
 double r11, r12, r13,
        r21, r22, r23,
        r31, r32, r33;

 r11 = gsl_matrix_get(r, 0, 0);
 r12 = gsl_matrix_get(r, 0, 1);
 r13 = gsl_matrix_get(r, 0, 2);
 r21 = gsl_matrix_get(r, 1, 0);
 r22 = gsl_matrix_get(r, 1, 1);
 r23 = gsl_matrix_get(r, 1, 2);
 r31 = gsl_matrix_get(r, 2, 0);
 r32 = gsl_matrix_get(r, 2, 1);
 r33 = gsl_matrix_get(r, 2, 2);

 if( r13*( r21*r32 - r22*r31 ) - r23*(r11*r32 - r31*r12)
     + r33*( r11*r22 - r12*r21 ) > 0 )
    return 1;
 else
    return 0;
}



