#include "ptrace.h"

static Color direct_light( int ns, Vector3 p, Vector3 n, Vector3 v,
                         Object *ol, PolyLight *l, Material *m );
static int light_visible(Ray r, Object *ol, PolyLight *l, Color *c );

#include "ptrace.h"


Color trace_path( int first_level, int level, Ray v, Object *ol )
{
 PolyLight *l = plight_head;
 Color c = C_BLACK, c_aux, w;
 Ray r;
 Inode *i = ray_intersect(ol, v);
 
 adjust_normal( i, v );
 if( light_visible(v,ol,l,&w) )
   return w;
   
 if((i != NULL) && level){
    level--;
    Material *m = i->m;
    Vector3 p = ray_point(v, i->t); 
    Cone  recv = cone_make(p, i->n, PIOVER2);
  
    if( m->se < 0 )
       c = mirror_reflect( first_level, level, v, i, p, ol, m );   
    else{
       Vector3 d = sort_new_direction(recv);
       c = direct_light( DIRECT_LIGHT_NSMPLS, p, i->n,
                         v3_scale(-1,v.d), ol, l, m);
       if( !light_visible(ray_make(recv.o, d), ol, l, &w) ){
         c_aux = trace_path( first_level, level, ray_make(p, d), ol );
         c = c_add(c, apply_bphong( m, c_aux, i->n,
                   d, v3_scale(-1,v.d), 2*PI*v3_dot(d, i->n) ) );
       } 
    }
    inode_free(i); 
    return c;
 }
 else{
   if( i == NULL )
      return C_BLACK;
   else{  
      inode_free(i);
      return C_BLACK;
   }
 }
}

Color direct_light( int ns, Vector3 p, Vector3 n, Vector3 v,
                      Object *ol, PolyLight *l, Material *m )  
{
 int k;
 Vector3 r, light_ptdir, inv_light_ptdir;
 Ray s;
 Color c, cl_light;
 Inode *i;
 double g=0;

 c = c_make(0,0,0);
 for( k = 0; k < ns; k++ ){ 
   s = plight_sample(l,&cl_light);
   r = v3_sub(s.o, p);
   light_ptdir = v3_unit(r);
   inv_light_ptdir = v3_scale( -1, light_ptdir );
   i = ray_intersect(ol, ray_make(p, light_ptdir));
   if( (i == NULL) || (i->t > v3_norm(r)) ){
      g = fabs( v3_dot(inv_light_ptdir,s.d)*v3_dot(light_ptdir, n) );
      c = c_add(c, apply_bphong(m, cl_light, n, light_ptdir, v, 
                   g*plight_area(l)/SQR(v3_norm(r) ) ) );
   }
   if( i != NULL )
      inode_free(i);
 } 

 return v3_scale(1./ns, c);
}   

int light_visible(Ray r, Object *ol, PolyLight *l, Color *c)
{
 Inode *i1, *i2;

 i1 = plight_intersect( l, r, c );
 if( i1 != NULL ){
   i2 = ray_intersect( ol, r );
   if( i2 == NULL || (i1->t < i2->t ) ){
     inode_free(i1);
     if( i2 != NULL )
       inode_free(i2);
     return TRUE;
   }
   inode_free(i1);
   if( i2 != NULL )
     inode_free(i2);
 }
 return FALSE;
}


