cmake_minimum_required(VERSION 3.10)


set(CMAKE_C_COMPILER /bin/clang)
set(CMAKE_CXX_COMPILER /bin/clang++)
set(CMAKE_CXX_STANDART 17)

project(Engine)

message("${PROJECT_SOURCE_DIR}/libs/")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/libs")

# main code
file(GLOB_RECURSE SOURCES src/*.c**)
file(GLOB_RECURSE HEADERS src/*.h**)

add_executable(Engine ${SOURCES} ${HEADERS})

# glfw stuff
if(UNIX)
    option(_GLFW_X11 "Use X11 window system" ON)
elseif(WIN32)
    option(_GLFW_WIN32 "Use windows API" ON)
elseif(APPLE)
    option(_GLFW_X11 "Use Apple stuff (needs to be redacted)" ON)
endif() 

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw/glfw-3.3.8)
target_link_libraries(Engine PRIVATE glfw)

add_subdirectory(libs/glad)
target_link_libraries(Engine PRIVATE glad)

add_subdirectory(libs/glm/glm-master)
target_link_libraries(Engine PRIVATE glm)

include_directories(libs/stb-master/stb-master)
link_directories(libs/stb-master/stb-master)

# assimp

find_package(assimp REQUIRED)
if(assimp_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
    target_link_libraries(Engine PRIVATE ${ASSIMP_LIBRARIES})
endif()
