
add_library( img STATIC image.c  pngfile.c image.h  )

install(FILES image.h pngfile.h  DESTINATION ${INCL_DIR})

install(TARGETS img  DESTINATION ${LIB_DIR})
