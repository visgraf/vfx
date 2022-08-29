#include "singlecam.h"

static int cost_func(const gsl_vector *x, void *params, gsl_vector *f);
static double reproj_error( const gsl_vector *x, gsl_matrix *points, gsl_matrix *projs, int point_index );


void singlecam_nlin_calib( gsl_vector *xout, gsl_vector *x, gsl_matrix *points, gsl_matrix *projs )
{
 const gsl_multifit_fdfsolver_type *t = gsl_multifit_fdfsolver_lmsder;
 gsl_multifit_fdfsolver *s;
 gsl_multifit_function_fdf f;
 int npoints = projs->size1;
 SingleCamData d;

 d.projs = projs;
 d.points = points;
 
 f.f = &cost_func;
 f.df = NULL;

 f.p = x->size;
 f.n = npoints;

 f.params = &d;

 s = gsl_multifit_fdfsolver_alloc(t, f.n, f.p);
 gsl_multifit_fdfsolver_set(s, &f, x);

 ba_optimize(s);
 
 gsl_vector_memcpy( xout, s->x );
 gsl_multifit_fdfsolver_free(s); 
}
 

int cost_func(const gsl_vector *x, void *params, gsl_vector *f)
{
 int j;
 gsl_matrix *projs;
 gsl_matrix *points;

 projs = ((SingleCamData*)params)->projs;
 points = ((SingleCamData*)params)->points;
 gsl_vector_set_zero(f);
 for( j = 0; j < projs->size1; j++ )
    gsl_vector_set( f, j, reproj_error( x, points, projs, j ) );

 return GSL_SUCCESS;
}


double reproj_error( const gsl_vector *x, gsl_matrix *points, gsl_matrix *projs, int point_index )
{
 double error;
 gsl_vector  *prj, *v, *v_prj;
 gsl_matrix *p;

 v = gsl_vector_alloc(3);
 v_prj = gsl_vector_alloc(2);
 prj = gsl_vector_alloc(2);
 p = gsl_matrix_alloc( 3,4 );

 gsl_matrix_get_row( prj, projs, point_index );
 gsl_matrix_get_row( v, points, point_index );
 singlecam_get_camera( p, x );
 calib_apply_P( p, v, v_prj );
 gsl_vector_sub( prj, v_prj );
 error = SQR( gsl_blas_dnrm2( prj ) );

 gsl_vector_free(v);
 gsl_vector_free(v_prj);
 gsl_vector_free(prj);
 gsl_matrix_free(p);
 
 return error;
}
