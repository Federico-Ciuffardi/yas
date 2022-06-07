########
# Misc #
########
cmake_minimum_required(VERSION 3.23.1)
project(yas)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # create `compile_commands.json` for autocompletion

#################
# Compile flags #
#################
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wpedantic -Wextra -fexceptions -Wno-missing-field-initializers -O0 -g3 -ggdb")
set(CMAKE_BUILD_TYPE DEBUG)

#########
# Paths #
#########

# Sources
file(GLOB_RECURSE MAIN_SOURCES
  src/*.cpp
  src/**/*.cpp
)

# Includes
include_directories(GLOB_RECURSE MAIN_INCLUDES
  src
  include
  lib
)

########
# Deps #
########

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# git2
find_package(git2 REQUIRED)
include_directories(${GIT2_INCLUDE_DIR})

# yaml-cpp
find_package(yaml-cpp)
include_directories(${YAML_INCLUDE_DIRS})

########
# Misc #
########

include_directories(${MAIN_INCLUDES})
add_executable(main ${MAIN_SOURCES})
target_link_libraries(main ${GIT2_LIBRARIES} ${YAML_CPP_LIBRARIES})
