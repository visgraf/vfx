#include "stack.h"
#include "hier.h"


static Stack4 *stk = NULL;


Val xaffine_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};
  if (pass == T_EXEC) { Pval *p; 
    Matrix4 m = m4_ident();
    Matrix4 mi = m4_ident();
    for (p = pl; p != NULL; p = p->next) {
      if (strcmp(p->name, "translation") == 0 && p->val.type == V_PVL ) {
	Vector3 v = pvl_to_v3(p->val.u.v);
	m = m4_m4prod(m, m4_translate( v.x, v.y, v.z));
	mi = m4_m4prod(m4_translate( -v.x, -v.y, -v.z), mi);
      } else if (strcmp(p->name, "rotation") == 0 && p->val.type == V_PVL ) {
	Vector3 rot = pvl_to_v3(p->val.u.v);
	m = m4_m4prod(m, m4_rotate('x', rot.x));
	mi = m4_m4prod(m4_rotate('x', -rot.x), mi);
	m = m4_m4prod(m, m4_rotate('y', rot.y));
	mi = m4_m4prod(m4_rotate('y', -rot.y), mi);
	m = m4_m4prod(m, m4_rotate('z', rot.z));
	mi = m4_m4prod(m4_rotate('z', -rot.z), mi);
      } else if (strcmp(p->name, "scaling") == 0 && p->val.type == V_PVL ) {
	Vector3 v = pvl_to_v3(p->val.u.v);
	m = m4_m4prod(m, m4_scale( v.x, v.y, v.z));
	mi = m4_m4prod(m4_scale( 1./v.x, 1./v.y, 1./v.z), mi);
      } else
	error("(xaffine) wrong argument");
    }
    v.u.v = xform_setmat(xform_new(XFORM_AFFINE), m, mi);
    v.type = V_TRANSFORM;
    s4_concmat(stk, &m, &mi);
  }
  return v;
}

Val xparam_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};
  if (pass == T_EXEC) { Pval *p;
    Transform *t;
    double par[9] = {0,0,0, 0,0,0, 1,1,1};
    for (p = pl; p != NULL; p = p->next) {
      if (p->name && strcmp(p->name, "rts") == 0 && p->val.type == V_PVL ) {
	pvl_to_array(p->val.u.v, par, 9);
      } else
	error("(xparam) wrong argument");
    }
    t = xform_new(XFORM_PARAM);
    xform_setpar(t, par);
    xform_updatepar(t);
    xform_applypar_m(t);
    xform_applypar_mi(t);
    v.u.v = t;
    v.type = V_TRANSFORM;
    s4_concmat(stk, &(t->m), &(t->mi));
  }
  return v;
}

void xform_push()
{
  if (stk == NULL)
    stk = s4_initstack(MAX_STK_DEPTH);
  s4_push(stk);
}

void xform_pop()
{
  s4_pop(stk);
}

Matrix4 xform_ctm()
{
  return s4_getmat(stk);
}

Matrix4 xform_ctmi()
{
  return s4_getimat(stk);
}

Transform *xform_new(int type)
{
  Transform *t = NEWSTRUCT(Transform);
  t->type = type;
  t->p_cb = (Pfun)(0);
  t->info = NULL;
  xform_reset(t);
  return t;
}

void xform_free(Transform *t)
{
  efree(t->info);
  efree(t);
}

Transform *xform_setmat(Transform *t, Matrix4 m, Matrix4 mi)
{
  t->m = m;
  t->mi = mi;
  return t;
}

Transform *xform_setpar(Transform *t, double *p)
{
  t->p[RX] = p[RX]; t->p[RY] = p[RY]; t->p[RZ] = p[RZ];
  t->p[TX] = p[TX]; t->p[TY] = p[TY]; t->p[TZ] = p[TZ];
  t->p[SX] = p[SX]; t->p[SY] = p[SY]; t->p[SZ] = p[SZ];
  return t;
}

Transform *xform_updatepar(Transform *t)
{
  if (t->p_cb)
    (t->p_cb)(t->p, 9, t->info);
  return t;
}

Transform *xform_applypar_m(Transform *t)
{
  Matrix4 m = m4_ident();

  m = m4_m4prod(m, m4_rotate('x', t->p[RX]));
  m = m4_m4prod(m, m4_rotate('y', t->p[RY]));
  m = m4_m4prod(m, m4_rotate('z', t->p[RZ]));

  m = m4_m4prod(m, m4_translate( t->p[TX], t->p[TY], t->p[TZ]));

  m = m4_m4prod(m, m4_scale( t->p[SX], t->p[SY], t->p[SZ]));

  t->m = m;
  return t;
}

Transform *xform_applypar_mi(Transform *t)
{
  Matrix4 mi = m4_ident();

  mi = m4_m4prod(m4_scale( 1./t->p[SX], 1./t->p[SY], 1./t->p[SZ]), mi);

  mi = m4_m4prod(m4_translate( -t->p[TX], -t->p[TY], -t->p[TZ]), mi);

  mi = m4_m4prod(m4_rotate('z', -t->p[RZ]), mi);
  mi = m4_m4prod(m4_rotate('y', -t->p[RY]), mi);
  mi = m4_m4prod(m4_rotate('x', -t->p[RX]), mi);

  t->mi = mi;
  return t;
}

void xform_reset(Transform *t)
{
  t->m = t->mi = m4_ident();
  t->p[RX] = t->p[RY] = t->p[RZ] = 0;
  t->p[TX] = t->p[TY] = t->p[TZ] = 0;
  t->p[SX] = t->p[SY] = t->p[SZ] = 1;
}

Matrix4 xform_m(Transform *t)
{
  if (t->type == XFORM_PARAM) {
    xform_updatepar(t);
    xform_applypar_m(t);
  }
  return t->m;
}

Matrix4 xform_mi(Transform *t)
{
  if (t->type == XFORM_PARAM) {
    xform_updatepar(t);
    xform_applypar_mi(t);
  }
  return t->mi;
}
