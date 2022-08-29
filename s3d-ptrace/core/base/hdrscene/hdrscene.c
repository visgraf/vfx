#include "hdrscene.h"


static struct View *view = NULL;
static struct HDRImage *hdrimg = NULL;
static struct HDRDome  *hdrdome = NULL;
static struct PolyLight *plights = NULL;
static struct PolyShadow *plshadow = NULL;
static struct Object *objs = NULL;

#ifndef LIST_INSERT
#define LIST_INSERT(L, I, TYPE) {struct TYPE *tt = I; tt->next = L; L = tt; }
#endif

static void hdrcollect_items(Pval *pl)
{
  Pval *p = pl;
  while (p != NULL) {
    if (p->name && strcmp(p->name,"object") == 0 && p->val.type == V_OBJECT) 
      objs = obj_insert(objs, p->val.u.v);
    else if (p->name && strcmp(p->name,"object") == 0 && p->val.type == V_GROUP)
      objs = obj_list_insert(objs, p->val.u.v);
    else if (p->name && strcmp(p->name,"camera") == 0 && p->val.type == V_CAMERA)
      view = p->val.u.v;
    else if (p->name && strcmp(p->name,"dome") == 0 && p->val.type == V_HDR_DOME)
      hdrdome = p->val.u.v;
    else if (p->name && strcmp(p->name,"light") == 0 && p->val.type == V_PL_LIGHT)
      LIST_INSERT(plights, p->val.u.v, PolyLight)
    else if(p->name && strcmp(p->name,"shadow") == 0 && p->val.type == V_PL_SHADOW) 
      LIST_INSERT(plshadow, p->val.u.v, PolyShadow)
    p = p->next;
  }
}


Val hdrscene_parse(int c, Pval *pl)
{
  Val v = {V_NULL, 0};
  
  if (c == T_EXEC) {
    HDRScene *s = NEWSTRUCT(HDRScene);
    view = NULL; hdrimg = NULL; plights = NULL; objs = NULL;

    hdrcollect_items(pl);
    s->view = view;
    s->hdrimg = hdrimg;
    s->hdrdome = hdrdome;
    s->objs = objs;
    s->plights = plights;
    s->plshadow = plshadow;
    v.type = V_HDR_SCENE;
    v.u.v = s;
  }
  return v;
}


HDRScene *hdrscene_read(void)
{
  if (lang_parse() == 0)
    return hdrscene_eval();
  else
    error("(hdrscene read)");
}


HDRScene *hdrscene_eval(void)
{
  HDRScene *s;
  Val v = lang_nd_eval();
  if (v.type != V_HDR_SCENE)
    error("(hdrscene eval)");
  else
    s =  v.u.v;
  if (s->view == NULL)
    s->view = initview();
  if (s->hdrimg == NULL)
    s->hdrimg = hdrimg_init( s->view->sc.ur.x, s->view->sc.ur.y );
  return s;
}

void hdrscene_free(HDRScene *s)
{
  if (s->objs)
    obj_list_free(s->objs);
  if (s->view)
    efree(s->view);
  if (s->hdrimg)
    hdrimg_free(s->hdrimg);
  if (s->hdrdome)
    hdrdome_free(s->hdrdome);
  if (s->plights)
    plight_free(s->plights);
  if(s->plshadow)
    plshadow_free(s->plshadow);
  efree(s);
}


