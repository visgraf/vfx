

add_library( lptrace  STATIC ptrace.c  dmptrace.c aux.c ptrace.h )

install(FILES ptrace.h  DESTINATION ${INCL_DIR})

install(TARGETS lptrace  DESTINATION ${LIB_DIR})
