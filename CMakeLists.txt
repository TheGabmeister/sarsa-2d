cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project ("pikuma-engine")

# imgui and sol requires modern c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)
#delete the out folder after changing if visual studio doesn recognize the change!

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL2_LIBRARY "E:/Personal_Projects/2025/Game Engine 2D/pikuma-2d-engine/build/Debug/SDL3.dll")  # Or SDL2.lib on Windows
set(SDL2_INCLUDE_DIR "E:/Personal_Projects/2025/Game Engine 2D/pikuma-2d-engine/thirdparty/SDL/include/SDL3") 

add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL) 
add_subdirectory(thirdparty/SDL_mixer EXCLUDE_FROM_ALL) 
add_subdirectory(thirdparty/SDL_ttf EXCLUDE_FROM_ALL) 

# Turn off unsupported image types. Only allow BMP, JPG, and PNG
set(SDLIMAGE_AVIF OFF CACHE BOOL "Support loading AVIF images" FORCE)
set(SDLIMAGE_GIF OFF CACHE BOOL "Support loading GIF images" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "Support loading JXL images" FORCE)
set(SDLIMAGE_LBM OFF CACHE BOOL "Support loading LBM images" FORCE)
set(SDLIMAGE_PCX OFF CACHE BOOL "Support loading PCX images" FORCE)
set(SDLIMAGE_PNM OFF CACHE BOOL "Support loading PNM images" FORCE)
set(SDLIMAGE_QOI OFF CACHE BOOL "Support loading QOI images" FORCE)
set(SDLIMAGE_SVG OFF CACHE BOOL "Support loading SVG images" FORCE)
set(SDLIMAGE_TGA OFF CACHE BOOL "Support loading TGA images" FORCE)
set(SDLIMAGE_TIF OFF CACHE BOOL "Support loading TIFF images" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "Support loading WEBP images" FORCE)
set(SDLIMAGE_XCF OFF CACHE BOOL "Support loading XCF images" FORCE)
set(SDLIMAGE_XPM OFF CACHE BOOL "Support loading XPM images" FORCE)
set(SDLIMAGE_XV OFF CACHE BOOL "Support loading XV images" FORCE)
add_subdirectory(thirdparty/SDL_image EXCLUDE_FROM_ALL)

add_subdirectory(thirdparty/glm EXCLUDE_FROM_ALL)

set(IMGUI_PATH  "thirdparty/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp) 
add_library(imgui STATIC ${IMGUI_SOURCES})

set(LUA_PATH  "thirdparty/lua")
file(GLOB LUA_SOURCES ${LUA_PATH}/*.c) 
add_library(lua STATIC ${LUA_SOURCES})

##############################################################################

# Automatically add all .cpp and .h
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable (${PROJECT_NAME} ${ENGINE_SOURCES}) 

if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./assets/") 

	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=0) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1) 

endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
	SDL3::SDL3
	SDL3_image::SDL3_image
	glm::glm-header-only
	lua
	imgui
)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

#target_include_directories(imgui PUBLIC ${IMGUI_PATH})