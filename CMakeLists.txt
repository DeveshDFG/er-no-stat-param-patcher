cmake_minimum_required(VERSION 3.21)
project(er-no-stat-param-patcher CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required for ABI compatibility with ELDEN RING / libER containers.
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

set(LIBER_LOCAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libER/libER")
if(NOT EXISTS "${LIBER_LOCAL_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "libER source was not found at ${LIBER_LOCAL_SOURCE_DIR}")
endif()

add_subdirectory("${LIBER_LOCAL_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/libER-local")

add_library(er-no-stat-param-patcher SHARED
    src/dllmain.cpp
    src/param_patch.cpp
)

target_include_directories(er-no-stat-param-patcher PRIVATE
    include
)

target_link_libraries(er-no-stat-param-patcher PRIVATE
    libER_static
)

set_target_properties(er-no-stat-param-patcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_SOURCE_DIR}/natives"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/natives"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/natives"
)

add_custom_command(TARGET er-no-stat-param-patcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Built DLL: $<TARGET_FILE:er-no-stat-param-patcher>"
)
