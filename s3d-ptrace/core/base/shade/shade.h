#ifndef SHADE_H
#define SHADE_H

#include "geom.h"
#include "color.h"
#include "prim.h"
#include "poly.h"
#include "view.h"
#include "image.h"
#include "sdltypes.h"


typedef struct Shader {
  char *name;
  char *vertp;
  char *fragp;
  unsigned int id;
} Shader;


typedef struct Cone {
  Vector3 o, d;
  Real cosa;
} Cone;

Cone cone_make(Vector3 o, Vector3 d, Real angle);
int point_coupling(Cone a, Cone b);
int dir_coupling(Cone a, Vector3 v);


#define LIGHT_AMBIENT 0
#define LIGHT_DISTANT 1
#define LIGHT_POINT   2
#define LIGHT_SPOT    3


typedef struct Light {
  struct Light *next;
  int          type;
  Color        color;
  Real         ambient;
  Real         intensity;
  Vector3      loc;
  Vector3      dir;
  Real         cutoff;
  Real         distr;
  Real         att0, att1, att2;
  Vector3      outdir;
  Color        outcol;
  int          (*transport)();
  void         *tinfo;
} Light;


typedef struct Material {
  Color c;
  Color s;
  Real  ka;
  Real  kd;
  Real  ks;
  Real  se;
  Real  kt;
  Real  ir;
  Color (*luminance)();
  void  *tinfo;
  Shader shd;
} Material;


typedef struct RContext {
  Vector3  v;
  Vector3  p;
  Vector3  n;
  Vector3  du;
  Vector3  dv;
  Vector3  t;
  Material *m;
  Light    *l;
  View     *c;
  Image    *img;
} RContext;


/* render context */

RContext *rc_set(RContext *rc, Vector3 v, Vector3 p, Vector3 n, Material *m);
RContext *rc_tset(RContext *rc, Vector3 v, Vector3 p, Vector3 n,
		  Vector3 t, Vector3 du, Vector3 dv, Material *m);
RContext *rc_sset(RContext *rc, View *v, Light *l, Image *img);

/* light */

int ambientlight(Light *l, Cone recv, RContext *rc);
int distantlight(Light *l, Cone recv, RContext *rc);
int pointlight(Light *l, Cone recv, RContext *rc);
int spotlight(Light *l, Cone recv, RContext *rc);

Val distlight_parse(int pass, Pval *pl);
Light *light_ambient(Real amb);

/* material */

Color matte(RContext *rc);
Color metal(RContext *rc);
Color plastic(RContext *rc);

Val plastic_parse(int pass, Pval *pl);

Material *mat_default(void);
void mat_free(Material *mat);

/* illum */

Color ambient(RContext *rc);
Color diffuse(RContext *rc);
Color specular(RContext *rc);
Color constant(RContext *rc);

/* shade */

Color point_shade(Vector3 p, Vector3 n, Vector3 v, RContext *rc, Material *m);
Color point_texshade(Vector3 p, Vector3 n, Vector3 v, RContext *rc, Material *m, Prim *s);

Color flat_shade(Hpoly *p, Vector3 v, RContext *rc, Material *m);

typedef struct FlatData {
  Image *img;
  Color c;
} FlatData;

void *flat_setshade(FlatData *d, Hpoly *p, Vector3 v, RContext *rc, Material *m);
void flat_paint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st, void *data);


typedef struct GouraudData {
  Image *img;
  Hpoly *cols;
} GouraudData;


void gouraud_shade(Hpoly *c, Hpoly *p, Hpoly *n, Vector3 v, RContext *rc, Material *m);
void *gouraud_set(GouraudData *g, Hpoly *c, Image *i);
void gouraud_paint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st,void *data);

typedef struct PhongData {
  Hpoly *pnts;
  Hpoly *norms;
  Vector3 v;
  RContext *rc;
} PhongData;

void *phong_set(PhongData *d, Hpoly *p, Hpoly *n, Vector3 v, RContext *rc, Material *m);
void phong_shadepaint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st, void *data);

#define col_dpymap(c) c_scale(255.0, c)

Shader make_shader(char *name, const char *vert, const char *frag);
Shader lookup_shader(char *name);
Shader read_shaderfiles(char *name);
Shader parse_shader(Pval *pl, char *defshader);
Val shaderprog_parse(int pass, Pval *pl);

extern const char* phong_vertp;
extern const char* phong_matte_fragp;

extern const char* render_context;
extern const char* light_support;
extern const char* illum_models;

#endif


