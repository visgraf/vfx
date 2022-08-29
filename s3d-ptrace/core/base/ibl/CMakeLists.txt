

add_library( ibl  STATIC hdr.c ibl.c plshadow.c lang.c ibl.h hdr.h )

install(FILES ibl.h hdr.h DESTINATION ${INCL_DIR})

install(TARGETS ibl DESTINATION ${LIB_DIR})
