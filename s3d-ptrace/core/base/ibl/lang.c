#include "ibl.h"
#include "sdltypes.h"


Val hdrdome_parse(int pass, Pval *pl)
{
  Pval *p;
  Val v = {V_NULL, 0};
    
  if (pass == T_EXEC) {
    Vector3 o, s; 
    HDRDome *dome;

    o = pvl_get_v3(pl, "orig", v3_make(1,0,0));
    s = pvl_get_v3(pl, "south", v3_make(0,-1,0));
    p = pl;
    while (p != NULL) {
      if (p->name && strcmp(p->name,"hdrimg") == 0 && p->val.type == V_STR)
	dome = hdrdome_alloc( o, s, hdrimg_read( (char*)p->val.u.v ) );
      p = p->next;
    }  
      
    v.type = V_HDR_DOME;
    v.u.v = dome;
    hdr_dome = (HDRDome*)dome;
  }
  return v;
}


Val plshadow_parse(int pass, Pval *plist)
{
 struct Hpoly *pols;
 Material *m;
 HDRImage *img = NULL;
 PolyShadow *plshadow;
 Val v = {V_NULL, 0};

 if (pass == T_EXEC) {
   Pval *p;

   for (p = plist; p !=NULL; p = p->next) {
     if (p->name == NULL) {
       error("(shadow) syntax error");
     } else if (strcmp(p->name, "material") == 0 && p->val.type == V_MATERIAL) {
         m  = p->val.u.v;
     } else if (strcmp(p->name, "shape") == 0 && p->val.type == V_HPOLYLIST) {
         pols = p->val.u.v;
     } else if( strcmp(p->name,"img") == 0 && p->val.type == V_STR ){ 
         img = hdrimg_read( (char*)p->val.u.v ); 
     } else error("(shadow) syntax error"); 
   }
   plshadow = plshadow_alloc( m, pols, img );
   v.type = V_PL_SHADOW;
   v.u.v = plshadow;
  }
  return v;
}

