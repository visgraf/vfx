
add_executable(modeling main.c)

target_link_libraries(modeling ${S3D_LIBRARIES} )

install(TARGETS modeling DESTINATION ${BIN_DIR})
