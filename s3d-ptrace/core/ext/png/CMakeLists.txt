
add_library(png STATIC  png.c   pngerror.c   pngget.c   pngmem.c   pngpread.c   pngread.c   pngrio.c   pngrtran.c   pngrutil.c   pngset.c    pngtrans.c   pngwio.c   pngwrite.c   pngwtran.c   pngwutil.c)

install(FILES png.h   pngconf.h   pngdebug.h   pnginfo.h pnglibconf.h   pngpriv.h   pngstruct.h DESTINATION ${INCL_DIR})

install(TARGETS png DESTINATION ${LIB_DIR})
