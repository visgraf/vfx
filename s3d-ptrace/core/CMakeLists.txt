# S3D

cmake_minimum_required(VERSION 2.8.11)

project(s3d)

set(VERSION 10.4.2)

set(CMAKE_BUILD_TYPE Debug)

set(BUILD_SHARED_LIBS false)

set(BASE ${PROJECT_SOURCE_DIR}/base) 
set(DEV ${PROJECT_SOURCE_DIR}/dev) 
set(EXT ${PROJECT_SOURCE_DIR}/ext) 
set(PROJ_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(SCN_DIR ${PROJ_ROOT_DIR}/scn)

set(CMAKE_INSTALL_PREFIX ${PROJ_ROOT_DIR}/platforms/linux)
set(BIN_DIR bin)
set(LIB_DIR lib)
set(INCL_DIR include)
 
set(DEBUG_C_FLAGS "-ggdb3")
set(RELEASE_C_FLAGS "-O3 -DNDEBUG -fopenmp -ffast-math -funroll-loops -fomit-frame-pointer -ftree-vectorize -fno-math-errno -funsafe-math-optimizations")

# set(COMMON_C_FLAGS "-Wall -Wextra -Wno-parentheses -Wno-unused -Wno-unknown-pragmas") 

if(APPLE)
   set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
   set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -D_APPLE_")
endif(APPLE)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${COMMON_C_FLAGS} ${RELEASE_C_FLAGS}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO  "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${COMMON_C_FLAGS} ${RELEASE_C_FLAGS} ${DEBUG_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} ${COMMON_C_FLAGS} ${DEBUG_C_FLAGS} -O0")

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")

find_package(BISON) 
find_package(FLEX)
 
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(ZLIB REQUIRED)

set(OPT_INCLUDE_DIR /opt/local/include)
include_directories(${OPT_INCLUDE_DIR})

# QT
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

include_directories(${BASE}/laux ${BASE}/geom
${BASE}/color ${BASE}/image ${BASE}/lang ${BASE}/poly ${BASE}/prim
${BASE}/csg ${BASE}/gener ${BASE}/view ${BASE}/clip ${BASE}/raster
${BASE}/shade ${BASE}/scene ${BASE}/hier ${BASE}/visib ${BASE}/ray
${BASE}/rad ${BASE}/map ${BASE}/anim ${BASE}/ibl 
${BASE}/plight ${BASE}/lptrace ${BASE}/hdrscene ${BASE}/lmmove 
${DEV}/glsl ${EXT}/png)

add_subdirectory(base) 
add_subdirectory(dev)  
add_subdirectory(ext)  
add_subdirectory(proj)


