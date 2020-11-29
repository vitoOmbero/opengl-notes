cmake_minimum_required(VERSION 3.9)

project(opengldemo LANGUAGES CXX)

# options
message("Set project options:")

option(DEBUG "Turn on debug features" ON)
option(VERBOSE_DEBUG "Turn on inlined stdout logger" ON)

message(STATUS "DEBUG: ${DEBUG}")
message(STATUS "VERBOSE_DEBUG: ${VERBOSE_DEBUG}")

message("Project options are set.")

# compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../00-libs")
set(COMMON_LIB_INCLUDE_DIRS
    ${COMMON_LIB_DIR}/glfw-3.3.2/include
    ${COMMON_LIB_DIR}/glew-2.1.0/include
    ${COMMON_LIB_DIR}/linux-x11/libglew)

set(SOURCES
    src/utils/terminal.cpp
    src/main_loop.cpp)

add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCES})

SET(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

if(DEBUG)
    if(VERBOSE_DEBUG)
        add_compile_definitions(KEY_INPUT_VERBOSE)
    endif(VERBOSE_DEBUG)
endif(DEBUG)

if(WIN32)
    add_compile_definitions(GLEW_BUILD=GLEW_STATIC)
endif(WIN32)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory( ${COMMON_LIB_DIR}/glfw-3.3.2/ ${COMMON_LIB_DIR}/linux-x11/libglfw/)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${COMMON_LIB_INCLUDE_DIRS}
    ${COMMON_LIB_DIR}/libglew/
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    GLEW
    glfw
    OpenGL::GL)

