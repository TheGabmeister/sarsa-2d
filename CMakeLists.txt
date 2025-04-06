cmake_minimum_required (VERSION 3.8)

project ("TG-Engine-2D")

add_executable (TG-Engine-2D "Main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TG-Engine-2D PROPERTY CXX_STANDARD 20)
endif()
