cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER /bin/clang)
set(CMAKE_CXX_COMPILER /bin/clang++)
set(CMAKE_CXX_STANDART 17)

project(Minivan)

file(GLOB_RECURSE SOURCES src/*.c**)
file(GLOB_RECURSE HEADERS src/*.h**)

add_executable(engine ${SOURCES} ${HEADERS})

#glfw stuff
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw/glfw-3.3.8)
target_link_libraries(engine PRIVATE glfw)

add_subdirectory(glad)
target_link_libraries(engine PRIVATE glad)

add_subdirectory(glm/glm-master)
target_link_libraries(engine PRIVATE glm)

include_directories(stb-master/stb-master)
link_directories(stb-master/stb-master)
