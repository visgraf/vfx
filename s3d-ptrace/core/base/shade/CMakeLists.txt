
add_library( shade STATIC  illum.c  shadelang.c  light.c  material.c misc.c shade.c  shade.h glshaders.c glshaders.h)

install(FILES shade.h  DESTINATION ${INCL_DIR})

install(TARGETS shade  DESTINATION ${LIB_DIR})
