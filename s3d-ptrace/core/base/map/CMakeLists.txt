
add_library( map STATIC  funcs.c  imagemap.c  maplang.c  lightmap.c  map.c  materialmap.c  shademap.c  map.h )

install(FILES map.h  DESTINATION ${INCL_DIR})

install(TARGETS map  DESTINATION ${LIB_DIR})
