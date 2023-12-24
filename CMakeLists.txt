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

########
# Deps #
########

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# git2
find_package(git2 REQUIRED)

# yaml-cpp
find_package(yaml-cpp)

##########
# Target #
##########

file(GLOB_RECURSE YAS_FILES
  src/*.cpp
  src/**/*.cpp
)

add_executable(yas ${YAS_FILES})

target_include_directories(yas PUBLIC
  include 
  lib 
  ${YAML_INCLUDE_DIRS}
  ${GIT2_INCLUDE_DIR}
)

target_link_libraries(yas ${GIT2_LIBRARIES} ${YAML_CPP_LIBRARIES})
