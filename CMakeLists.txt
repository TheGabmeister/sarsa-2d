cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project ("TG-Engine-2D")

# imgui and sol requires modern c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Third-party directories
set(SDL_DIR "${PROJECT_SOURCE_DIR}/thirdparty/SDL")

include_directories("${SDL_DIR}/include")
link_directories("${SDL_DIR}/lib/x64")

add_executable (TG-Engine-2D "Main.cpp" "Game.h" "Game.cpp")

target_link_libraries( ${PROJECT_NAME} PRIVATE 
  "${SDL_DIR}/lib/x64/SDL3.lib"
)

# Add .dlls into location of .exe
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${SDL_DIR}/lib/x64/SDL3.dll" 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)