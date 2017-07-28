cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Type of build")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

project(ProjectName)

include(ExternalProject)

ExternalProject_Add(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG master

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""

  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DBUILD_SHARED_LIBS=OFF
             -DSFML_BUILD_NETWORK=OFF
)

ExternalProject_Get_Property(SFML SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

add_subdirectory(src)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Jam")
