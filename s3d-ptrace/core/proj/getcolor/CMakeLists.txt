
add_executable(getcolor main.c)

target_link_libraries(getcolor ${S3D_LIBRARIES} -lgomp )

install(TARGETS getcolor DESTINATION ${BIN_DIR})
