cmake_minimum_required(VERSION 3.0)
project(GLBase VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(
    "deps/glew/include"
    "deps/glfw/include"
    "deps/imgui/include"
    "include"
)
add_subdirectory(deps)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/*.c*)

add_library(GLBase STATIC
    ${CORE_SOURCES}
)


target_link_libraries(GLBase
    glew_s
    imgui
    glfw
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_DIR  ${CMAKE_SOURCE_DIR}/src)

add_executable(glExplorer 
    ${SOURCE_DIR}/UserParams.cpp
    main.cpp
)
if(${WIN32} AND NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    if(${MSVC})
        target_link_options(glExplorer PRIVATE /SUBSYSTEM:WINDOWS)
        else()
        target_link_options(glExplorer PRIVATE -mwindows)
    endif(${MSVC})
    target_sources(glExplorer PRIVATE res.rc)
endif(${WIN32} AND NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")

add_executable(tests tests.cpp)
target_link_libraries(tests GLBase)
target_link_libraries(glExplorer GLBase)