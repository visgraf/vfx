#include "ba.h"

static int lm_max_iterations = LM_MAX_ITERATIONS;


void ba_get_proj( gsl_vector *prj, gsl_matrix *projs, int cam_index, int point_index )
{
 gsl_vector_set( prj, 0, gsl_matrix_get( projs, point_index, 2*cam_index ) );
 gsl_vector_set( prj, 1, gsl_matrix_get( projs, point_index, 2*cam_index + 1 ) );
}


void ba_set_proj( gsl_matrix *projs, gsl_vector *prj, int cam_index, int point_index )
{
 gsl_matrix_set( projs, point_index, 2*cam_index, gsl_vector_get( prj, 0 ) );
 gsl_matrix_set( projs, point_index, 2*cam_index + 1, gsl_vector_get( prj, 1 ));
}


void ba_get_point( gsl_vector *v, const gsl_vector *x, int point_index )
{
 gsl_vector_set( v, 0, gsl_vector_get( x, 3*point_index ));
 gsl_vector_set( v, 1, gsl_vector_get( x, 3*point_index + 1));
 gsl_vector_set( v, 2, gsl_vector_get( x, 3*point_index + 2));
}


void ba_set_point( gsl_vector *x, gsl_vector *v, int point_index )
{
 gsl_vector_set( x, 3*point_index , gsl_vector_get(v, 0) );
 gsl_vector_set( x, 3*point_index + 1 , gsl_vector_get(v,1) );
 gsl_vector_set( x, 3*point_index + 2 , gsl_vector_get(v,2) );
}


void ba_set_lm_max_iterations( int n )
{
 lm_max_iterations = n;
}
 

void ba_optimize( gsl_multifit_fdfsolver *s )
{
 int status, iter = 0;

 do{
    iter++;
    status = gsl_multifit_fdfsolver_iterate(s);
    if(status)
       break;
    status = gsl_multifit_test_delta(s->dx, s->x, LM_EPS, LM_EPS);
    printf( "%i Error = %f\n", iter, gsl_blas_dnrm2(s->f) ); 
 }
 while( (status == GSL_CONTINUE) && (iter <  lm_max_iterations ) );
}




