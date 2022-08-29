#ifndef HDR_SCENE_H
#define HDR_SCENE_H

#include <stdio.h>
#include "lang.h"
#include "obj.h"
#include "sdltypes.h"
#include "view.h"
#include "scene.h"
#include "hdr.h"
#include "ibl.h"
#include "plight.h"
#include "ibl.h"

typedef struct HDRScene {
  struct View   *view;
  struct HDRImage  *hdrimg;
  struct HDRDome  *hdrdome;
  struct PolyLight *plights;
  struct PolyShadow *plshadow;
  struct Object *objs;
} HDRScene; 


Val hdrscene_parse(int c, Pval *pl);
HDRScene *hdrscene_read(void);
HDRScene *hdrscene_eval(void);
void hdrscene_free(HDRScene *s);

#endif
