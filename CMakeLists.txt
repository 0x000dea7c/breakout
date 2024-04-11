cmake_minimum_required(VERSION 3.21)

set(PRECOMPILED_HEADER_CXX "${CMAKE_CURRENT_SOURCE_DIR}/inc/pch.hpp")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

# need to get rid of fucking conversion issues, don't know how though
set(
  CXX_FLAGS
  -Wall
  -Wextra # reasonable and standard
  -Wshadow # warn the user if a variable declaration shadows one from a parent context
  -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
  -Wold-style-cast # warn for c-style casts
  -Wcast-align # warn for potential performance problem casts
  -Wunused # warn on anything being unused
  -Woverloaded-virtual # warn if you overload (not override) a virtual function
  -Wpedantic # warn if non-standard C++ is used
  -Wconversion # warn on type conversions that may lose data
  -Wsign-conversion # warn on sign conversions
  -Wnull-dereference # warn if a null dereference is detected
  -Wdouble-promotion # warn if float is implicit promoted to double
  -Wformat=2 # warn on security issues around functions that format output (ie printf)
  -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
  -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
  -Wduplicated-cond # warn if if / else chain has duplicated conditions
  -Wduplicated-branches # warn if if / else branches have duplicated code
  -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
  -Wuseless-cast # warn if you perform a cast to the same type
  -march=native
  -pipe
)

project(
  breakout-lev
  VERSION 0.0.1
  DESCRIPTION "Breakout"
  LANGUAGES CXX C)

add_subdirectory(src)

find_package(SDL2_mixer REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

find_program(MOLD_EXE NAMES mold)

if(MOLD_EXE)
  target_link_options(breakout-lev PRIVATE "-fuse-ld=mold")
else()
  message("Mold not found, using default linker")
endif()

add_library(glad STATIC third_party/src/glad.c)
target_include_directories(glad PRIVATE third_party/inc)

add_library(stb_image STATIC third_party/src/stb_image.c)
target_include_directories(stb_image PRIVATE third_party/inc)

target_precompile_headers(breakout-lev PRIVATE
  ${PRECOMPILED_HEADER_CXX}
)

target_include_directories(breakout-lev PRIVATE
  inc
  third_party/inc)

target_include_directories(breakout-lev PRIVATE inc)

target_compile_options(breakout-lev PRIVATE ${CXX_FLAGS})

target_link_libraries(breakout-lev PRIVATE glad stb_image glfw SDL2 SDL2_mixer Freetype::Freetype)
