cmake_minimum_required (VERSION 3.10 FATAL_ERROR)

project (whereami)

add_library (${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/whereami.c
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
