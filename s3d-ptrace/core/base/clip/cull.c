#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "geom.h"
#include "poly.h"
#include "clip.h"


int is_backfacing(Hpoly *p, Vector3 v)
{
  return (v3_dot(hpoly_normal(p), v) >= 0)? TRUE : FALSE;
}

int clipcode(Real h, Vector3 v)
{
  int c = 0;
  if (v.y > -v.z) c |=  01;
  if (v.y <  v.z) c |=  02;
  if (v.x > -v.z) c |=  04;
  if (v.x <  v.z) c |= 010;
  return(c);
}

int inside_frustrum(Real h, Vector3 v)
{
  return (clipcode(h, v) == 0);
}

int cull_poly4(Real h, Hpoly *p)
{
  int i, c[3];
  for (i = 0; i < p->n; i++)
    c[i] = clipcode(h, v3_v4conv(p->v[i]));
  return (c[0] & c[1] & c[2]);
}

