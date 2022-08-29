
add_executable(mmove main.c)

target_link_libraries(mmove ${S3D_LIBRARIES} -lgomp )

install(TARGETS mmove DESTINATION ${BIN_DIR})
