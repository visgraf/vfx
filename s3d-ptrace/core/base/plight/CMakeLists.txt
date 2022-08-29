

add_library( plight  STATIC plight.c plight.h )

install(FILES plight.h  DESTINATION ${INCL_DIR})

install(TARGETS plight  DESTINATION ${LIB_DIR})
