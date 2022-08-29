
add_library(glsl STATIC  loadprogram.c opengl.c opengl.h shadertable.c shadertable.h)

install(FILES opengl.h DESTINATION ${INCL_DIR})

install(TARGETS glsl DESTINATION ${LIB_DIR})

