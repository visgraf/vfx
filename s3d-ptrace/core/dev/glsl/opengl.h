
#ifndef _OPENGL_H
#define _OPENGL_H

#ifdef __cplusplus
extern "C"{
#endif
    
#include "geom.h"
#include "poly.h"
#include "scene.h"
#include "shade.h"
#include "stack.h"
#include "map.h"
#include "hier.h"

#ifdef __cplusplus
}
#endif

void init_gl(Scene *s);

void setup_scene_gl(Scene *s);

void load_objects_gl(Object *objs);
void display_gl(Scene *s);

void close_gl(Scene *s);

void setup_programs_gl(Scene *s, Object *ol);
void cleanup_programs_gl();

void vbo_store_mesh_gl(Object *o, iMesh *m);

void set_material_gl(Material *m);

void set_render_to_fbo_gl(int flag);

#endif
