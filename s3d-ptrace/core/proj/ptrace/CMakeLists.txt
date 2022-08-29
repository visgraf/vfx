
add_executable(ptrace main.c)

target_link_libraries(ptrace ${S3D_LIBRARIES} -lgomp )

install(TARGETS ptrace DESTINATION ${BIN_DIR})
