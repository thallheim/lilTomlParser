cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_C_COMPILER "/usr/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # ninja/makefile only

project(lilTomlParser)

set(SRC)
file(GLOB SRC
  include/*.h include/*.hpp
  src/*.c src/*.cpp
)


add_library(ltp STATIC ${SRC})

add_compile_options(
  Wall
  Wextra
  Wflexible-array-extensions
  Wfor-loop-analysis
  $<$<CONFIG:Debug>:
    ggdb
    gcall-site-info>
)

target_compile_definitions(ltp PRIVATE
  $<$<CONFIG:Release>:NDEBUG>

)

