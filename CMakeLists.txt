cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (TG-Engine-2D)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# the generic targets ALL_BUILD, ZERO_CHECK, INSTALL, PACKAGE and RUN_TESTS will be grouped into the CMakePredefinedTargets folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
  lua
  URL https://www.lua.org/ftp/lua-5.4.7.tar.gz
  URL_HASH SHA256=9fbf5e28ef86c69858f6d3d34eccc32e911c1a28b4120ff3e84aaa70cfbf1e30
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(lua)

FetchContent_Declare(
  sol
  GIT_REPOSITORY https://github.com/ThePhD/sol2.git
  GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol)

FetchContent_Declare(
  sdl
  URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.8/SDL3-devel-3.2.8-VC.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(sdl)

FetchContent_Declare(
  sdl-image
  URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-devel-3.2.4-VC.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(sdl-image)

FetchContent_Declare(
  sdl-ttf
  URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.0/SDL3_ttf-devel-3.2.0-VC.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(sdl-ttf)

FetchContent_Declare(
  sdl-mixer
  URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-VC.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(sdl-mixer)

add_executable (${PROJECT_NAME} "TG-Engine-2D.cpp" "TG-Engine-2D.h")
target_link_libraries(${PROJECT_NAME} PRIVATE glm imgui lua sol sdl sdl-image sdl-ttf sdl-mixer)