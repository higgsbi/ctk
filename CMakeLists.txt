﻿cmake_minimum_required(VERSION 3.5)
project(ctk)

option(LOCAL_BUILD "Build library locally instead of to system" OFF)

set(SOURCES 
	src/types/string.c
	src/file/path.c
	src/file/file.c
	src/io.c
	src/math/aabb.c
	src/math/vectors.c
	src/math/matrix.c
)

set(HEADERS 
	src/io.h
	src/types/string.h
	src/types/types.h
	src/file/path.h
	src/file/file.h
	src/collections/vector.h
	src/collections/entry.h
	src/collections/entries.h
	src/collections/map.h
	src/math/aabb.h
	src/math/vectors.h
	src/math/math.h
	src/math/matrix.h
)

if (LOCAL_BUILD)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
endif()

if(NOT CMAKE_BUILD_TYPE) 
	set(CMAKE_BUILD_TYPE Release) 
endif() 

set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -s") 
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,--gc-sections -flto")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g") 
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wno-unused-function -Wno-pointer-sign)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/" DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}"
        FILES_MATCHING
        PATTERN "*.h")
