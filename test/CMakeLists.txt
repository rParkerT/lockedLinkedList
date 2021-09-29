
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
          git_catch
          PREFIX ${CMAKE_BINARY_DIR}/catch2
          GIT_REPOSITORY https://github.com/catchorg/Catch2.git
          GIT_TAG        v2.13.1
          GIT_SHALLOW 1
          TIMEOUT 10
          CONFIGURE_COMMAND ""
          BUILD_COMMAND ""
          INSTALL_COMMAND ""
          LOG_DOWNLOAD ON)

ExternalProject_Get_Property(git_catch source_dir)
set(CATCH_PATH ${source_dir}/single_include
${source_dir}/single_include/catch2
${source_dir}/catch2
${source_dir}/include/internal/benchmark)

if(build_tests_COMPILED_CATCH)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/pre_compiled_catch.cpp)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/pre_compiled_catch.cpp
            "#define CATCH_CONFIG_MAIN\n#include <catch.hpp>\n")
endif()

# Make sure it's visible if it's a shared object.
set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)
add_library(catch SHARED
            ${CMAKE_CURRENT_BINARY_DIR}/pre_compiled_catch.cpp)

target_include_directories(catch PUBLIC ${CATCH_PATH})
else()
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CATCH_PATH})
endif()

if(TARGET git_catch)
# If cloning, make sure it's cloned BEFORE it's needed.
add_dependencies(catch git_catch)
endif()
find_package(Threads REQUIRED)

include_directories(${LockedLinkedList_SOURCE_DIR}/impl)

add_executable(tests
              catchMain.cpp
              testLockedLinkedList.cpp
)

target_link_libraries(tests PUBLIC catch Threads::Threads)
