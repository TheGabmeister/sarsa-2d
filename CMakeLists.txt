cmake_minimum_required (VERSION 3.16)

project ("TG-Engine-2D")

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)

add_executable (TG-Engine-2D "Main.cpp" "Game.h" "Game.cpp")

# Link to the actual SDL3 library.
target_link_libraries(TG-Engine-2D PRIVATE SDL3::SDL3)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TG-Engine-2D PROPERTY CXX_STANDARD 20)
endif()
