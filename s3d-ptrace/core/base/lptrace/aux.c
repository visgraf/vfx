#include "ptrace.h"


Color mirror_reflect( int first_level, int level, Ray v, Inode *i, 
                      Vector3 p, Object *ol, Material *m ) 
{
 Vector3 d, c_aux;
 
 d = v3_sub( v.d, v3_scale( 2*v3_dot( v.d, i->n ), i->n ) );
 c_aux = trace_path( first_level, level, ray_make(p, d), ol );
 return v3_scale( m->ks, c_aux );
}


Color dome_mirror_reflect( int first_level, int level, Ray v, Inode *i, 
                      Vector3 p, Object *ol, Material *m ) 
{
 Vector3 d, c_aux;
 
 d = v3_sub( v.d, v3_scale( 2*v3_dot( v.d, i->n ), i->n ) );
 c_aux = dome_trace_path( first_level, level, ray_make(p, d), ol );
 return v3_scale( m->ks, c_aux );
}


Color apply_bphong( Material *m, Color lcolor, Vector3 n, Vector3 l,
                          Vector3 v, Real geom_fac)
{
 Color cs,cd;
 Vector3 h = v3_unit(v3_add(l,v));

 cs = c_scale(m->ks* pow(MAX(0, v3_dot(h,n)),m->se)*geom_fac, lcolor);
 cd = c_mult(m->c, c_scale(m->kd*geom_fac, lcolor) );
 return c_add( cs, cd );
}


Vector3 sort_new_direction( Cone c )
{
 double x, y, z;
 Vector3 v, n = v3_unit(c.d);

 do{
   x = 2*REAL_RANDOM - 1; 
   y = 2*REAL_RANDOM - 1; 
   z = 2*REAL_RANDOM - 1; 
   v = v3_make(x,y,z);
 }
 while( v3_norm(v) > 1 );
 
 if( v3_dot(v, c.d) < 0 ){
    double s = v3_dot(v3_scale(-1,v), n);
    v = v3_add( v, v3_scale(2*s,n) ); 
 }
 return v3_unit(v);
}


Bool hit_surface( Ray v, Object *ol )
{
 Inode *i = ray_intersect(ol, v);
 if( i == NULL )
   return FALSE;
 else{
   inode_free(i);
   return TRUE;
 }  
}


void adjust_normal( Inode *i, Ray v )
{
 if( i != NULL && v3_dot( i->n, v.d ) > 0 )
    i->n = v3_scale( -1, i->n );
}

