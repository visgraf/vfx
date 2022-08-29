#include "ptrace.h"
                        

Color dome_trace_path( int first_level, int level, Ray v, Object *ol )
{
 HDRDome *dome = hdr_dome;
 Color c = C_BLACK, c_aux, w;
 Ray r;
 Inode *i = ray_intersect(ol, v);
 
 adjust_normal( i, v );
 if((i != NULL) && level){
    level--;
    Material *m = i->m;
    Vector3 p = ray_point(v, i->t); 
    Cone  recv = cone_make(p, i->n, PIOVER2);
    Vector3 d = sort_new_direction(recv);
     
    if( m->se < 0 )
       c = dome_mirror_reflect( first_level, level, v, i, p, ol, m );  
    else{                   
       c_aux = dome_trace_path( first_level, level, ray_make(p, d), ol );
       c = apply_bphong( m, c_aux, i->n,  d, v3_scale(-1,v.d),
                         2*PI*v3_dot(d, i->n) );   
    }
    inode_free(i); 
    return c;
 }
 else{
   if(( i == NULL ) && (level == first_level ) ){
      return C_BLACK;
   } 
   else{
      if( i == NULL )
        return hdrdome_value( dome, v.d ); 
      if( i != NULL )
        inode_free(i);
      return C_BLACK;
   }  
 }
}




