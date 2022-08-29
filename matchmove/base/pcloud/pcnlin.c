#include "pcloud.h"


typedef struct PCloudData{
  Cameras *cams;
  PointCloud *pc;
  int nframes;
  int point;
} PCloudData;


static int cost_func(const gsl_vector *x, void *params, gsl_vector *f );
static float reproj_point_error( gsl_vector *x, PCloudData *data , int frame );
static void pcnlin_optimize( gsl_multifit_fdfsolver *s  );


void pcloud_nlin_optimize( gsl_vector *xout, gsl_vector *x, Cameras *cams,
                        PointCloud *pc, int nframes, int point )
{
 const gsl_multifit_fdfsolver_type *t = gsl_multifit_fdfsolver_lmsder;
 gsl_multifit_fdfsolver *s;
 gsl_multifit_function_fdf f;
 PCloudData d;

 f.f = &cost_func;
 f.df = NULL;
 f.p = 3;
 f.n = nframes;
 
 d.cams = cams;
 d.pc = pc;
 d.nframes = nframes;
 d.point = point;
 
 f.params = &d;
  
 s = gsl_multifit_fdfsolver_alloc(t, f.n, f.p);
 gsl_multifit_fdfsolver_set(s, &f, x);

 pcnlin_optimize(s);
 gsl_vector_memcpy( xout, s->x );
 gsl_multifit_fdfsolver_free(s);
}


int cost_func(const gsl_vector *x, void *params, gsl_vector *f)
{
 int i;
 PCloudData *data = (PCloudData*)params;
 
 gsl_vector_set_zero(f); 
 for( i = 0; i < data->nframes; i++ )
    gsl_vector_set( f, i, reproj_point_error( x, data, i ) );
     
 return GSL_SUCCESS;
}


float reproj_point_error( gsl_vector *x, PCloudData *data, int frame )
{
 float error;
 gsl_vector *p, *x_proj;
 
 p = gsl_vector_alloc(2);
 x_proj = gsl_vector_alloc(2);
 
 cameras_apply( data->cams, x, x_proj, data->pc[frame].frame );
 pcloud_get_point( p, &(data->pc[frame]), data->point );
 gsl_vector_sub( x_proj, p );
 error = gsl_blas_dnrm2( x_proj );
 
 gsl_vector_free( p );
 gsl_vector_free( x_proj );
 return error;
}


void pcnlin_optimize( gsl_multifit_fdfsolver *s )
{
 int status, iter = 0;

 do{
    iter++;
    status = gsl_multifit_fdfsolver_iterate(s);
    if(status)
       break;
    status = gsl_multifit_test_delta(s->dx, s->x, LM_EPS, LM_EPS);
 }
 while( (status == GSL_CONTINUE) && (iter < LM_MAX_ITERATIONS ) );
}
