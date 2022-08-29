#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "image.h"
#include "defs.h"
#include "geom.h"
#include "stack.h"
#include "view.h"
#include "poly.h"
#include "prim.h"
#include "hier.h"
#include "lang.h"
#include "clip.h"
#include "raster.h"
#include "shade.h"
#include "ray.h"
#include "csg.h"
#include "rt.h"
#include "rshade.h"

#include "scene.h"
#include "ptrace.h"
#include "ibl.h"

void init_render(Scene *s);
Ray ray_view(int u, int v);
void init_lang(void);


