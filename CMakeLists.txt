cmake_minimum_required(VERSION 3.0)
project(dungeon-cycle CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
if (EMSCRIPTEN)
  set(PLATFORM Web CACHE BOOL "" FORCE)
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
  GIT_TAG "5.5"
  GIT_PROGRESS TRUE
)
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(raylib json)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --shell-file ../../template.html")
    # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

#add_compile_options(-Wall -Wextra -pedantic)
file(GLOB_RECURSE APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)
add_executable(${CMAKE_PROJECT_NAME} ${APP_SOURCES} ${WIN32_RESOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
else()
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE true)

  add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  )
  add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)
  target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set(WIN32_RESOURCES assets/windows/cycles.rc)
endif()

# Set version number
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC VERSION="1.0.0")
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_NAME="${CMAKE_PROJECT_NAME}")

if (EMSCRIPTEN)
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file assets")
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE raylib nlohmann_json::nlohmann_json)
