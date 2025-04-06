cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project ("TG-Engine-2D")

# imgui and sol requires modern c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Third-party directories
set(SDL_DIR "${PROJECT_SOURCE_DIR}/thirdparty/SDL")

include_directories("${SDL_DIR}/include")
link_directories("${SDL_DIR}/lib/x64")

add_executable (TG-Engine-2D "Main.cpp" "Game.h" "Game.cpp")

target_link_libraries( ${PROJECT_NAME} PRIVATE 
  "${SDL_DIR}/lib/x64/SDL3.lib"
)