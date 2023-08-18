cmake_minimum_required(VERSION 3.5.0)
project(colf)
set(EXE_NAME colf)

if(WIN32)
    include_directories(lib/SDL2-2.26.3/include)
    link_directories(SYSTEM lib/SDL2-2.26.3/lib/x64)

    include_directories(lib/SDL2_image-2.0.5/include)
    link_directories(SYSTEM lib/SDL2_image-2.0.5/lib/x64)

    include_directories(lib/SDL2_mixer-2.6.3/include)
    link_directories(SYSTEM lib/SDL2_mixer-2.6.3/lib/x64)

    include_directories(lib/cglm-0.8.3/include)
    link_directories(SYSTEM lib/cglm-0.8.3/build)

    include_directories(lib/glad/include/)  
set(GLAD_SOURCES lib/glad/src/glad.c)
endif(WIN32)
#include_directories(lib/cute)
file(COPY src/run_data DESTINATION .)

include_directories(src)

set(SOURCES
        src/assert.c src/assert.h
        src/main.c
        src/utils.c src/utils.h
        src/game/level.c src/game/level.h
        src/containers/stretchy_buffer.c src/containers/stretchy_buffer.h
        src/graphics/model.c src/graphics/model.h
        src/graphics/sprite.c src/graphics/sprite.h
        src/graphics/text.c src/graphics/text.h
        src/graphics/texture.c src/graphics/texture.h
        src/graphics/renderer.c src/graphics/renderer.h
        src/game/entity.c src/game/entity.h
        src/game/game_instance.c src/game/game_instance.h
        src/game/entity_functions.c
        src/graphics/gl_shader.c src/graphics/gl_shader.h
        src/disk.c src/disk.h
        src/containers/dynstring.c src/containers/dynstring.h)


if(APPLE)
    include_directories(lib/cglm-0.9.1/include)
    link_directories(lib/cglm-0.9.1/build)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif(APPLE)

add_executable(${EXE_NAME} ${SOURCES} ${GLAD_SOURCES})
set_property(TARGET ${EXE_NAME} PROPERTY C_STANDARD 11)

if(APPLE) 
    MESSAGE(status ">>> MAC")
    target_link_libraries(${EXE_NAME} SDL2_image::SDL2_image SDL2::SDL2 SDL2_ttf::SDL2_ttf m "-framework OpenGL")
    add_definitions(-DGL_SILENCE_DEPRECATION)
    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	foreach(dir ${dirs})
  		message(STATUS "dir='${dir}'")
	endforeach()

endif(APPLE)
if(WIN32)
    target_compile_options(${EXE_NAME} PRIVATE)
    target_link_libraries(${EXE_NAME} SDL2 SDL2_image SDL2_mixer)
endif (WIN32)
