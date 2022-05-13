########
# Misc #
########
cmake_minimum_required(VERSION 3.23.1)
project(yas)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # create `compile_commands.json` for autocompletion

#################
# Compile flags #
#################
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wpedantic -Wextra -fexceptions -O0 -g3 -ggdb")
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
)

########
# Deps #
########

########
# Misc #
########

include_directories(${MAIN_INCLUDES})
add_executable(main ${MAIN_SOURCES})
set_property(TARGET main PROPERTY CXX_STANDARD 20)
