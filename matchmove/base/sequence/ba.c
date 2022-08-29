#include "sequence.h"

static void  set_frame_projs( gsl_matrix *projs_buffer,  gsl_matrix *projs, int cam_index );
static void set_all_points( gsl_vector *params, gsl_matrix *points );
static void get_all_points( gsl_matrix *points, gsl_vector *params );


void subseq_extrinsic_ba( SubSeqNode *s, gsl_matrix *k )
{
 int i;
 gsl_vector *xout, *params, *t, *raxis;
 gsl_matrix  *projs, *r;

 xout = ba_ext_param_alloc( s->nframes, s->wnfeatures  );
 params = ba_ext_param_alloc( s->nframes, s->wnfeatures  );
 projs = gsl_matrix_alloc( s->wnfeatures,  2*s->nframes );
 raxis = gsl_vector_alloc( 3 );
 r = gsl_matrix_alloc( 3, 3 );
 t = gsl_vector_alloc( 3 );

 printf( "Bundle Adjustment...\n" );
 set_all_points( params, s->wpoints );
 for( i = 0; i < s->nframes; i++ ){
    ba_r_to_axis_angle( raxis,  s->cl[i].r  );
    ba_ext_set_camera( params, raxis, s->cl[i].t, s->wnfeatures, i );
    set_frame_projs( projs, WPROJS( s, i ), i );
 }

 ba_ext_exec( xout, params, projs , k );

 for( i = 0; i < s->nframes; i++ ){
      ba_ext_get_rt( raxis, t, xout, s->wnfeatures,  i );
      ba_axis_angle_to_r( r, raxis );
      camera_set( &(s->cl[i]), k,  r, t );
 }
 get_all_points( s->wpoints, params );

 gsl_vector_free( xout );
 gsl_vector_free( params );
 gsl_matrix_free( projs );
 gsl_vector_free( raxis );
 gsl_matrix_free( r );
 gsl_vector_free( t );
}


void  set_frame_projs( gsl_matrix *projs_buffer,  gsl_matrix *projs, int cam_index )
{
 int i;
 gsl_vector *proj;

 proj = gsl_vector_alloc( 2 );
 for( i = 0; i < projs->size1; i++ ){
    gsl_matrix_get_row(  proj, projs, i );
    ba_set_proj( projs_buffer, proj, cam_index, i );
 }
 
 gsl_vector_free( proj );
}


void set_all_points( gsl_vector *params, gsl_matrix *points )
{
 int i;
 gsl_vector *v;
 
 v = gsl_vector_alloc( 3 );
 for( i = 0; i < points->size1; i++ ){
   gsl_matrix_get_row( v, points, i );
   ba_set_point( params, v, i );
 }

 gsl_vector_free( v );   
}


void get_all_points( gsl_matrix *points, gsl_vector *params )
{
 int i;
 gsl_vector *v;
 
 v = gsl_vector_alloc( 3 );
 for( i = 0; i < points->size1; i++ ){
   ba_get_point( v, params, i );
   gsl_matrix_set_row( points, i,  v );
 }

 gsl_vector_free( v );   
}


void camera_set( Camera *c, gsl_matrix *k, gsl_matrix *r, gsl_vector *t )
{
 gsl_matrix_memcpy( c->k, k );
 gsl_matrix_memcpy( c->r, r );
 gsl_vector_memcpy( c->t, t ); 
}

