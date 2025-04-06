cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project ("TG-Engine-2D")

# imgui and sol requires modern c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/SDL_image EXCLUDE_FROM_ALL)

add_executable (${PROJECT_NAME} "Main.cpp" "Game.h" "Game.cpp")

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE 
	SDL3::SDL3
	SDL3_image::SDL3_image
)