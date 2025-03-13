cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TG-Engine-2D")

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

add_executable (TG-Engine-2D "TG-Engine-2D.cpp" "TG-Engine-2D.h")
target_link_libraries(${PROJECT_NAME} glm)