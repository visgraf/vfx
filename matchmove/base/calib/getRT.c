#include "calib.h"

char infront( gsl_matrix *pid, gsl_matrix *k, gsl_matrix *r,
              gsl_vector *t, gsl_vector *x1, gsl_vector *x2 );


#define COPY_RT( R, RSRC, T, TSRC )  { gsl_matrix_memcpy( R, RSRC ); gsl_vector_memcpy( T, TSRC ); }

void calib_ematrix_get_RT( gsl_matrix *k, gsl_matrix *e, gsl_vector *x1,
                           gsl_vector *x2, gsl_matrix *r, gsl_vector *tr )
{
 gsl_vector *t;
 gsl_matrix *r1, *r2, *id, *pid;

 r1 = gsl_matrix_alloc(3,3);
 r2 = gsl_matrix_alloc(3,3);
 id = gsl_matrix_alloc(3,3);
 pid = gsl_matrix_alloc(3,4);
 t = gsl_vector_alloc(3);

 gsl_vector_set_zero(t);
 gsl_matrix_set_zero(id);
 gsl_matrix_set( id, 0,0,1.);
 gsl_matrix_set( id, 1,1,1.);
 gsl_matrix_set( id, 2,2,1.);

 calib_pmatrix_make( pid, k, id, t );
 calib_ematrix_get_all_rt( e, r1, r2, t );

 if( infront( pid, k, r1, t, x1, x2 ) )
   COPY_RT( r, r1, tr, t )
 if( infront( pid, k, r2, t, x1, x2 ) )
   COPY_RT( r, r2, tr, t )
 gsl_vector_scale( t, -1. );
 if( infront( pid, k, r1, t, x1, x2 ) )
   COPY_RT( r, r1, tr, t )
 if( infront( pid, k, r2, t, x1, x2 ) )
   COPY_RT( r, r2, tr, t )

 gsl_matrix_free(r1);
 gsl_matrix_free(r2);
 gsl_matrix_free(id);
 gsl_matrix_free(pid);
 gsl_vector_free(t);
}


char infront( gsl_matrix *pid, gsl_matrix *k, gsl_matrix *r,
              gsl_vector *t, gsl_vector *x1, gsl_vector *x2 )
{
 gsl_vector *x,*xcam;
 gsl_matrix *p;

 x = gsl_vector_alloc(3);
 p = gsl_matrix_alloc(3, 4);
 calib_pmatrix_make( p, k, r, t );
 calib_get_3dpoint( pid, p, x1, x2, x );
 gsl_matrix_free(p);

 if( gsl_vector_get(x, 2) < 0 ){
    gsl_vector_free(x);
    return 0;
 }
 else{
    gsl_vector *xcam;
    xcam = gsl_vector_alloc(3);
    calib_rt_transform( xcam, r, t, x );
    if( gsl_vector_get(xcam, 2) < 0 ){
       gsl_vector_free(x); gsl_vector_free(xcam);
       return 0;
    }
 }
 return 1;
}

