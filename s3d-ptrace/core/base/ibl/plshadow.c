#include "ibl.h"


PolyShadow *plshadow_alloc( Material *m, Hpoly *p, HDRImage *img )
{
 PolyShadow *ps;

 ps = NEWSTRUCT(PolyShadow);
 ps->m = m;
 ps->p = p;
 ps->img = img;
 return ps;
}


void plshadow_free( PolyShadow *ps )
{
 free( ps->m );
 free( ps->p );
 free( ps->img );
 free( ps );
}


Object *plshadow_to_obj( PolyShadow *ps ) 
{
 Shape *sh = shape_new(V_HPOLYLIST, ps->p);
 Object *o = obj_new(V_HPOLYLIST, sh);
 o->mat = ps->m;
 
 return o;
}

