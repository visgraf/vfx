

add_library( lmmove  STATIC cdata.c mmove.c mmove.h )

install(FILES mmove.h  DESTINATION ${INCL_DIR})

install(TARGETS lmmove  DESTINATION ${LIB_DIR})
