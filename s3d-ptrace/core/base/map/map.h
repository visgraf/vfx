#ifndef MAP_H
#define MAP_H

#include "geom.h"
#include "color.h"
#include "poly.h"
#include "shade.h"
#include "lang.h"

#define TEX_MAXPAR 6

#define TEX_W 0
#define TEX_T 1
#define TEX_P 2
#define TEX_N 3
#define TEX_U 4 
#define TEX_V 5

Hpoly *texture_wscale(Hpoly *w, Hpoly *p);
Hpoly  *poly_wz_hpoly(Hpoly *w, Hpoly *s);

typedef struct TextureData {
  Hpoly *vpar[TEX_MAXPAR];
  RContext *rc;
} TextureData;

void texture_shadepaint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st, void *data);
void *texture_set(TextureData *d, Hpoly *w, Hpoly *t, Hpoly *p, Hpoly *n, Hpoly *u, Hpoly *v,
		  RContext *rc, Material *m);

typedef struct TextureSrc {
  Color (*texfunc)();
  unsigned char type;
  unsigned int  gltex;
  void *texdata;
} TextureSrc;

// texture type
#define TSRC_IMAGE 10
#define TSRC_PROC  20


typedef struct TmapInfo {
  TextureSrc *src;
  unsigned char code;
  Color bg;
} TmapInfo;

// texture code
#define TMAP_TILE 1
#define TMAP_CLAMP 3

Color image_texture(Image *i, Vector3 t);

Color textured_plastic(RContext *rc);
Color texture_default();

TextureSrc *parse_texsource(Pval *pl);
Val imagemap_parse(int pass, Pval *pl);
Val textured_parse(int pass, Pval *pl);

Color texture_map(void *info, Vector3 t);
Color bump_map(void *info, Vector3 t, Vector3 n, Vector3 ds, Vector3 dt);
Color environment_map(void *info, Vector3 r);

Color rough_surface(RContext *rc);
Val rough_parse(int pass, Pval *pl);

Color shiny_surface(RContext *rc);
Val shiny_parse(int pass, Pval *pl);

typedef struct TslideInfo {
  TextureSrc *src;
  Vector3 u, v;
} TslideInfo;

int slide_projector(Light *l, Cone recv, RContext *rc);
Val slideproj_parse(int pass, Pval *pl);

typedef struct ChequerInfo {
  Real xfreq, yfreq;
  Color fg, bg;
} ChequerInfo;

Real mod(Real a, Real b);
Color chequer(Real xfreq, Real yfreq, Color a, Color b, Vector3 t);
Val chequer_parse(int pass, Pval *pl);
Color chequer_texture(ChequerInfo *c,  Vector3 t);

#endif


