#ifndef CLIP_H
#define CLIP_H

int is_backfacing(Hpoly *p, Vector3 v);
int cull_poly4(Real h, Hpoly *p);
int inside_frustrum(Real h, Vector3 v);

int poly_clip(Hpoly *p, void (*dispose)(void*), int chain);

int hither_clip(Real h, Hpoly *p, void (*render)(), void (*plfree)());

#endif
