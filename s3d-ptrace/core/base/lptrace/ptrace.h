#ifndef PTRACE_H
#define PTRACE_H


#include <math.h>
#include "rshade.h"
#include "rt.h"
#include "ibl.h"
#include "plight.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>


Color trace_path( int first_level, int level, Ray v, Object *ol );
Color dome_trace_path( int first_level, int level, Ray v, Object *ol );
Color mirror_reflect( int first_level, int level, Ray v, Inode *i, 
                      Vector3 p, Object *ol, Material *m );
Color dome_mirror_reflect( int first_level, int level, Ray v, Inode *i, 
                           Vector3 p, Object *ol, Material *m );
Color apply_bphong( Material *m, Color lcolor, Vector3 n, Vector3 l,  Vector3 v, Real geom_fac );
Vector3 sort_new_direction( Cone c );
Bool hit_surface( Ray v, Object *ol );
void adjust_normal( Inode *i, Ray v );

#endif
