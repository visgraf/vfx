#include "ba.h"

static int ba_ext_cost_func(const gsl_vector *x, void *params, gsl_vector *f);
static int ba_ext_diff_func(const gsl_vector *x, void *params, gsl_matrix *J);

static gsl_matrix *kmatrix;


void ba_ext_exec( gsl_vector *xout, gsl_vector *x, gsl_matrix *projs, gsl_matrix *k )
{
 const gsl_multifit_fdfsolver_type *t = gsl_multifit_fdfsolver_lmsder;
 gsl_multifit_fdfsolver *s;
 gsl_multifit_function_fdf f;
 BundleAdjustData d;

 d.projs = projs;
 d.n_points = projs->size1;
 d.n_cameras = projs->size2/2;

 f.f = &ba_ext_cost_func;
 f.df = NULL;
 kmatrix = k;

 f.p = 3*d.n_points + 6*d.n_cameras ;
 f.n = d.n_cameras * d.n_points;
 f.params = &d;

 s = gsl_multifit_fdfsolver_alloc(t, f.n, f.p);
 gsl_multifit_fdfsolver_set(s, &f, x);

 ba_optimize(s);
 
 gsl_vector_memcpy( xout, s->x );
 gsl_multifit_fdfsolver_free(s);
}


int ba_ext_cost_func(const gsl_vector *x, void *params, gsl_vector *f)
{
 int i,j;
 BundleAdjustData *data;
 //retirar a linha abaixo
 double global_error = 0;

 data = (BundleAdjustData*)params;
 for( i = 0; i < data->n_cameras; i++ )
   for( j = 0; j < data->n_points; j++ ){
      gsl_vector_set( f, i*data->n_points + j, ba_ext_reproj_error( x, data->projs, i, j ) );
   }

 return GSL_SUCCESS;
}


/* error between estimated projection and real image projection*/
double ba_ext_reproj_error( const gsl_vector *x, gsl_matrix *projs, int cam, int point_index )
{
 double error;
 gsl_vector *prj, *v, *v_prj;
 gsl_matrix *p;

 v = gsl_vector_alloc(3);
 v_prj = gsl_vector_alloc(2);
 prj = gsl_vector_alloc(2);
 p = gsl_matrix_alloc(3,4);

 ba_get_proj( prj, projs, cam, point_index );
 ba_ext_get_camera( p, x, projs->size1, cam, kmatrix );
 ba_get_point( v , x, point_index );
 calib_apply_P( p, v, v_prj );
 gsl_vector_sub( prj, v_prj );
 error = SQR( gsl_blas_dnrm2( prj ) );
 
 gsl_vector_free(v);
 gsl_vector_free(v_prj);
 gsl_vector_free(prj);
 gsl_matrix_free(p);

 return error;
}
