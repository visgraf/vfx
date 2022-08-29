#include "plight.h"



void init_plight_list( void )
{
 plight_head = NULL;
}


PolyLight *plight_alloc( Color c, Hpoly *p )
{
 PolyLight *pl;

 pl = NEWSTRUCT(PolyLight);
 pl->next = NULL;
 pl->p = p;
 pl->c = c;
 return pl;
}


void plight_free( PolyLight *head )
{
 PolyLight *pl;

 for( pl = head; pl != NULL; pl=pl->next ){
   free( pl->p );
   free( pl );
 }
}


void plight_insert( PolyLight *pl )
{
 pl->next = plight_head; 
 plight_head = pl;
}


Ray plight_sample( PolyLight *pl, Color *c )
{
 int i, r, n = 0;
 PolyLight *aux;
 
 for( aux = pl; aux != NULL; n++, aux = aux->next );
 r = rand()%n; 
 for( aux = pl, i=0; i < r-1; aux = aux->next );

 *c = aux->c;
 return poly3_sample(aux->p);
}


Real plight_area( PolyLight *pl )
{
 Real a = 0;

 while (pl != NULL) {
  a += hpoly3_area(pl->p);
  pl = pl->next;
 }
 return a;
} 


Inode *plight_intersect( PolyLight *pl, Ray r, Color *c )
{
 Inode *l = NULL;   
 
 while( pl != NULL && l == NULL ){
   l = hpoly_intersect(pl->p, hpoly3_plane(pl->p), r);
   *c = pl->c;
   pl = pl->next;
 }
 return l;
}


Val plight_parse(int pass, Pval *plist)
{
 Color c;
 struct Hpoly *pl, *pols;
 Val v = {V_NULL, 0};

 if (pass == T_EXEC) {
   Pval *p;

   for (p = plist; p !=NULL; p = p->next) {
     if (p->name == NULL) {
       error("(extense light) syntax error");
     } else if (strcmp(p->name, "color") == 0) {
         c = pvl_to_v3(p->val.u.v);
     } else if (strcmp(p->name, "shape") == 0 && p->val.type == V_HPOLYLIST) {
         pols = p->val.u.v;
     } else error("(extense light) syntax error"); 
   }
   for(pl = pols; pl != NULL; pl = pl->next) 
      plight_insert( plight_alloc( c, pl ));
   v.type = V_PL_LIGHT;
   v.u.v = NULL;
  }
  return v;
}

Ray poly3_sample( Hpoly *p )
{
 Vector3 a, b, c, v0, v1, v2;
 Real r1 = REAL_RANDOM; 
 Real r2 = REAL_RANDOM;
 
 v0 = v3_make(p->v[0].x, p->v[0].y, p->v[0].z); 
 v1 = v3_make(p->v[1].x, p->v[1].y, p->v[1].z);  
 v2 = v3_make(p->v[2].x, p->v[2].y, p->v[2].z);   
  
 a = v3_scale(1 - sqrt(r1), v0);
 b = v3_scale(sqrt(r1)*(1 - r2), v1);
 c = v3_scale(sqrt(r1)*r2, v2);

 return ray_make( v3_add(a, v3_add(b, c)), hpoly_normal(p)); 
}


