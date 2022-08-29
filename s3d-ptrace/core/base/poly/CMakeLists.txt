
add_library( poly STATIC hpoly.c  poly.c poly.h )

install(FILES poly.h  DESTINATION ${INCL_DIR})

install(TARGETS poly   DESTINATION ${LIB_DIR})
