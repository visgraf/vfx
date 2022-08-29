#ifndef RELAX_CAMERAS_H
#define RELAX_CAMERAS_H

#include "rcontext.h"
#include "relax_points.h"
#include "ba.h"


void relax_cameras( gsl_vector *xout, RelaxationContext *rc, int cam_id );
void adjust_relax_context( RelaxationContext *rc, gsl_vector *xout, int cam_id );
             
#endif
