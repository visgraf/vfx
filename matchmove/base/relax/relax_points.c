#include "rcontext.h"
#include "relax_points.h"


typedef struct RelaxData{
 HomologPoint *hp; 
 Cameras *c;
} RelaxData;

static int relax_cost_func( const gsl_vector *x, void *params, gsl_vector *f);
static double relax_reproj_error( const gsl_vector *x, int i, RelaxData *d );


void relax_points( gsl_vector *xout, gsl_vector *x, HomologPoint *hp, Cameras *c )
{
 const gsl_multifit_fdfsolver_type *t = gsl_multifit_fdfsolver_lmsder;
 gsl_multifit_fdfsolver *s;
 gsl_multifit_function_fdf f;
 RelaxData d;
 
 d.hp = hp;
 d.c = c;
 
 f.f = &relax_cost_func;
 f.df = NULL;
 f.p = 3;
 f.n = hp->last_frame - hp->first_frame + 1;
 if( f.n < f.p ){
   gsl_vector_memcpy( xout, x );
   return;
 }
 
 f.params = &d;
  
 s = gsl_multifit_fdfsolver_alloc(t, f.n, f.p);
 gsl_multifit_fdfsolver_set(s, &f, x);
 relax_optimize(s);
 gsl_vector_memcpy( xout, s->x );
 gsl_multifit_fdfsolver_free(s);
}
 
 
int relax_cost_func(const gsl_vector *x, void *params, gsl_vector *f)
{
 int i;
 RelaxData *data = (RelaxData*)params;
  
 gsl_vector_set_zero(f);
 for( i = data->hp->first_frame; i <= data->hp->last_frame; i++ )
   if( hp_is_inlier( data->hp, i ) ){
     gsl_vector_set( f, i - data->hp->first_frame, 
                     relax_reproj_error( x, i, data ));
   }
    
 return GSL_SUCCESS;
}


double relax_reproj_error( const gsl_vector *x, int i, RelaxData *data )
{
 double error;
 gsl_vector *proj, *ref_proj;
 
 proj = gsl_vector_alloc(2);
 ref_proj = gsl_vector_alloc(2);
  
 cameras_apply( data->c, x, proj, i );
 
 gsl_vector_set( ref_proj, 0, gsl_matrix_get( data->hp->projs, i - data->hp->first_frame, 0 )); 
 gsl_vector_set( ref_proj, 1, gsl_matrix_get( data->hp->projs, i - data->hp->first_frame, 1 ));
 gsl_vector_sub( proj, ref_proj );
 error = gsl_blas_dnrm2( proj );
 
 gsl_vector_free(proj);
 gsl_vector_free(ref_proj);
 return error;
}


void relax_optimize( gsl_multifit_fdfsolver *s )
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
 while( (status == GSL_CONTINUE) && (iter < LM_MAX_ITERATIONS ) );
}



