cmake_minimum_required (VERSION 3.10 FATAL_ERROR)

project (whereami-cpp)

add_subdirectory(external/whereami EXCLUDE_FROM_ALL)

add_library (${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/whereami++.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    whereami
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/whereami/src
)
