

add_library( hdrscene  STATIC hdrscene.c  hdrscene.h )

install(FILES hdrscene.h  DESTINATION ${INCL_DIR})

install(TARGETS hdrscene  DESTINATION ${LIB_DIR})
