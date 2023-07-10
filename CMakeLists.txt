# Thank you siddharthroy12

cmake_minimum_required(VERSION 3.0)
project(cycles CXX) # TODO Project name

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
if (EMSCRIPTEN)
  set(PLATFORM Web CACHE BOOL "" FORCE)
endif()
#set(GRAPHICS GRAPHICS_API_OPENGL_11 CACHE BOOL "" FORCE) # use opengl 1.1

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_subdirectory(libs/raylib)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --shell-file ../../template.html")
    # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)

# if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#     enable_language("RC")
#     set(WIN32_RESOURCES cycles.rc)
# endif()

#add_compile_options(-Wall -Wextra -pedantic)
file(GLOB_RECURSE APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)
add_executable(${CMAKE_PROJECT_NAME} ${APP_SOURCES} ${WIN32_RESOURCES})

# Conditionally set the WIN32 property if not debug
if(NOT CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE true)
endif()

add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)

# Set version number
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC VERSION="1.0.0")
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_NAME="${CMAKE_PROJECT_NAME}")

if (EMSCRIPTEN)
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file resources")
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE raylib)
