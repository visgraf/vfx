#include "sequence.h" 

#define SCALE_MAX_ITERATIONS 50
#define SCALE_OPTIMIZE_RADIUS 1.01
#define MIN_ACCEPTABLE_NINLIERS 3


typedef struct ScaleOptimizeContext{
  int *reproj_inliers;
  int n;  /* p1 and p2 number of elements */
  int *p1;
  int *p2;
  SubSeqNode *subseq;
}ScaleOptimizeContext;


static double get_scale( SubSeqNode *s );
static int get_good_vector( int *good, SubSeqNode *s );
static void get_inlier_index( int *v , int *inlier, int *good, int n);
static int scale_reproj_inliers( int *reproj_inliers, gsl_vector *error, double scale, SubSeqNode *s, int *p1, int *p2 );
static double scale_optimize( int *reproj_inliers, double scale, SubSeqNode *s, int *p1, int *p2, int n_interframe_inliers );
static double scale_cost_func( double x, void *params );
static double mean_global_error( gsl_vector *error, int *reproj_inliers );


void get_scale_vector( double *scale_vector, CalibSequenceContext *cc )
{
 int i = 0;
 SubSeqNode *aux; 

 for( aux = cc->sl; (aux != NULL) && (aux->next != NULL) && (aux->next->next != NULL); aux = aux->next )
     scale_vector[i++] = get_scale(aux);
}


double get_scale( SubSeqNode *s )
{
 double scale, best_scale = 1;
 double global_error, min_global_error = MAX_DOUBLE;
 int best_ninliers, ninliers, i, k;
 int good[MAX_INTERFRAG_NINLIERS];
 int p1[MAX_INTERFRAG_NINLIERS];
 int p2[MAX_INTERFRAG_NINLIERS];
 int *reproj_inliers, *best_inliers;
 gsl_vector *error;

 k = get_good_vector( good, s );
 get_inlier_index( p1, s->winliers, good, s->cc->nfeatures );
 get_inlier_index( p2, s->next->winliers, good, s->cc->nfeatures);
 reproj_inliers = NEWTARRAY( k, int );
 best_inliers = NEWTARRAY( k, int );
 error = gsl_vector_alloc(k);

 printf( "Shared Inliers %i\n", k );
 printf( "Frame: %i to %i\n", s->first_frame, s->last_frame ); 
 for( scale = EPS; scale < 100.; scale *= SCALE_OPTIMIZE_RADIUS ){
    ninliers = scale_reproj_inliers( reproj_inliers, error, scale, s, p1, p2 );
    if( (global_error = mean_global_error( error, reproj_inliers )) <= min_global_error ){
          best_ninliers = ninliers;
          best_scale = scale;
          min_global_error = global_error;
          memcpy( best_inliers, reproj_inliers, k*sizeof(int) ); 
    }
 }
 printf( "best scale: %f , ninliers = %i, error = %f\n", best_scale, best_ninliers, min_global_error/s->nframes );
 best_scale = scale_optimize( best_inliers, best_scale, s, p1, p2, k );

 free( reproj_inliers );
 free( best_inliers );
 gsl_vector_free( error );
 return best_scale;
}


static int get_good_vector( int *good, SubSeqNode *s )
{
 int i, k = 0;

 for( i = 0; i < s->cc->nfeatures; i++ )
    if( s->winliers[i] == INLIER && s->next->winliers[i] == INLIER ){
      good[k++] = i;
    } 
 return k;
}


static void get_inlier_index( int *v , int *inlier, int *good, int n )
{
 int i, j=0, k=0, w = 0;
 
 for( i = 0; i < n; i++ ){
   if( inlier[i] == INLIER ){
     if( i == good[j] )
       v[j++] = k;
     k++;
   }
 }
}


static int scale_reproj_inliers( int *reproj_inliers, gsl_vector *error, double scale, SubSeqNode *s, int *p1, int *p2 )
{
 int i,j, n, w, ninliers = 0;
 gsl_vector *v, *v_proj, *real_proj, *t;
 gsl_matrix *id, *r, *ref_a1, *ref_a2, *ref_b1, *ref_b2, *p;

 v = gsl_vector_alloc( 3 ); 
 v_proj = gsl_vector_alloc( 2 );
 real_proj = gsl_vector_alloc( 2 );
 t = gsl_vector_alloc( 3 );
 id = gsl_matrix_alloc( 3,3 );
 r = gsl_matrix_alloc( 3, 3 );
 ref_a1 = gsl_matrix_alloc( 3, 4 );
 ref_a2 = gsl_matrix_alloc( 3, 4 );
 ref_b1 = gsl_matrix_alloc( 3, 4 );
 ref_b2 = gsl_matrix_alloc( 3, 4 );
 p = gsl_matrix_alloc( 3, 4 );

 gsl_matrix_set_identity(id);
 n = s->nframes - 1;
 gsl_vector_memcpy( t, s->next->cl[0].t );
 gsl_vector_scale( t, scale );
 calib_pmatrix_make( ref_a1, id, s->next->cl[0].r, t );
 calib_pmatrix_make( ref_a2, id, s->cl[n].r, s->cl[n].t );

 for( i = 0; i < error->size; i++ ){
  w = 0;
  gsl_vector_set( error, i, 0. );
  gsl_matrix_get_row( v, s->wpoints, p1[i] );
  for( j = 0; j < s->next->nframes; j++ ){
     gsl_vector_memcpy( t, s->next->cl[j].t );
     gsl_vector_scale( t, scale );
     calib_pmatrix_make( ref_b1, id, s->next->cl[j].r, t );
     calib_change_ref( ref_a1, ref_a2, ref_b1, ref_b2  );
     extract_rt( r, t, ref_b2 );
     calib_pmatrix_make( p, s->next->cl[j].k, r, t );
     calib_apply_P( p, v, v_proj );
     gsl_matrix_get_row( real_proj, WPROJS( s->next, j ), p2[i] );
     gsl_vector_sub( v_proj, real_proj );
     gsl_vector_set( error, i, gsl_blas_dnrm2( v_proj ) + gsl_vector_get(error,i) );
     if( gsl_blas_dnrm2( v_proj )  < INTERFRAME_REPROJ_TOL )
        w++;
   }
   if( w == s->next->nframes ){
     ninliers++; 
     reproj_inliers[i] = INLIER;
   }
   else
     reproj_inliers[i] = OUTLIER;
 }

 gsl_vector_free( v );
 gsl_vector_free( v_proj );
 gsl_vector_free( real_proj );
 gsl_vector_free( t );
 gsl_matrix_free( id );
 gsl_matrix_free( r );
 gsl_matrix_free( ref_a1 );
 gsl_matrix_free( ref_a2 );
 gsl_matrix_free( ref_b1 );
 gsl_matrix_free( ref_b2 );
 gsl_matrix_free( p );
 return ninliers;
}


static double scale_optimize( int *reproj_inliers, double scale, SubSeqNode *s, int *p1, int *p2, int n ) 
{
 int status, iter = 0;
 double a = EPS;
 double b = 100.;
 const gsl_min_fminimizer_type *T;
 gsl_min_fminimizer *m;
 gsl_function F;
 ScaleOptimizeContext sc;

 sc.reproj_inliers = reproj_inliers;
 sc.n = n; 
 sc.p1 = p1;
 sc.p2 = p2;
 sc.subseq = s;

 F.function = &scale_cost_func;
 F.params = &sc;

 T = gsl_min_fminimizer_brent;
 m = gsl_min_fminimizer_alloc(T);
 gsl_min_fminimizer_set (m, &F, scale, a, b );

 do{
  iter++;
  status = gsl_min_fminimizer_iterate(m);
  scale = gsl_min_fminimizer_x_minimum(m);
  a = gsl_min_fminimizer_x_lower(m);
  b = gsl_min_fminimizer_x_upper(m);
  status = gsl_min_test_interval (a, b, 1e-10, 0.0);
 }
 while (status == GSL_CONTINUE && iter < SCALE_MAX_ITERATIONS );
 gsl_min_fminimizer_free(m);
 
 return scale;
}


double scale_cost_func( double x, void *params )
{
 int i,j;
 double global_error = 0;
 int *inliers;
 gsl_vector *error;
 ScaleOptimizeContext *sc;

 sc = (ScaleOptimizeContext*)params;
 inliers = NEWTARRAY( sc->n , int );
 error = gsl_vector_alloc( sc->n );

 scale_reproj_inliers( inliers, error, x, sc->subseq, sc->p1, sc->p2 );
 global_error = mean_global_error( error, inliers );

 free( inliers );
 gsl_vector_free( error );
 return global_error;
}


double mean_global_error( gsl_vector *error, int *reproj_inliers )
{
 int i, n=0;
 double global_error = 0;

 for( i=0; i < error->size; i++ )
   if( reproj_inliers[i] == INLIER ){
     global_error += gsl_vector_get( error, i );
     n++; 
   }
 
 if( n > MIN_ACCEPTABLE_NINLIERS ) 
   return global_error/n; 
 else
   return MAX_DOUBLE;
}

