cmake_minimum_required(VERSION 3.14)

# Project definitions
project(news-skeleton
  VERSION 0.1
  DESCRIPTION "Example client/server library"
  LANGUAGES CXX
)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler and linker options
set(COMPILER_FLAGS_COMMON "-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wno-switch" CACHE STRING "Common compiler flags for all builds")

set(C_FLAGS_COMMON   "${COMPILER_FLAGS_COMMON}" CACHE STRING "C compiler flags for all builds")
set(CXX_FLAGS_COMMON "${COMPILER_FLAGS_COMMON}" CACHE STRING "C++ compiler flags for all builds")

set(CMAKE_C_FLAGS_DEBUG          "${CMAKE_C_FLAGS_DEBUG} -Og ${C_FLAGS_COMMON}")
set(CMAKE_C_FLAGS_RELEASE        "${CMAKE_C_FLAGS_RELEASE} ${C_FLAGS_COMMON}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${C_FLAGS_COMMON}")

set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -Og ${CXX_FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CXX_FLAGS_COMMON}")

# Default to RelWithDebInfo build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

# Output compiler commands, used by some editors
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(clientserver "")  # empty, sources to be added by target_sources

add_subdirectory(src)

# variables for common settings to avoid repeating the same details for multiple targets
 SET(clientserver_public_include_dirs
         $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
 )
 SET(clientserver_private_include_dirs
         ${CMAKE_CURRENT_SOURCE_DIR}/src
         ${PROJECT_BINARY_DIR}/src   # for generated files (e.g. clientserver_config.h) in build dir
 )
 
target_include_directories(clientserver
    PUBLIC
        ${clientserver_public_include_dirs}
    PRIVATE
        ${clientserver_private_include_dirs}
)

### variable used when configuring files
##get_filename_component(src_dir_path "${PROJECT_SOURCE_DIR}" ABSOLUTE)


##################### Compiler and linker options ############

# build with sanitizers in debug builds
set(clientserver_sanitizer_options "$<$<CONFIG:Debug>:-fsanitize=address,undefined>")

target_compile_options(clientserver PRIVATE ${clientserver_sanitizer_options})
target_link_libraries(clientserver PRIVATE ${clientserver_sanitizer_options})

# ##################### Build type, etc ########################

# # we default to Release build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# # output build type and compiler flags
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
# message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
# message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
# message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")

# message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
# message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
# message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
# message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")

##################### examples are in a subdirectory ############

add_subdirectory(example) # Add these late to make flags, etc. visible


# Print summary of configuration

set(MSG_CMAKE_SUMMARY "CMake Summary")

# CMake version numbers
string(APPEND MSG_CMAKE_SUMMARY "\n\
   |-- Version Information --\n\
   |   |-- Installed on build platform:        ${CMAKE_VERSION}\n\
   |   `-- Required to generate build system:  ${CMAKE_MINIMUM_REQUIRED_VERSION}\n\
   |"
)

# Build type and according compiler flags
string(APPEND MSG_CMAKE_SUMMARY "\n\
   `-- Build Information --\n\
       |-- Type: ${CMAKE_BUILD_TYPE}\n\
       |-- C flags\n\
       |   |-- Debug:\n\
       |   |     ${CMAKE_C_FLAGS_DEBUG}\n\
       |   |-- Release:\n\
       |   |     ${CMAKE_C_FLAGS_RELEASE}\n\
       |   `-- RelWithDebInfo (Release with debug info):\n\
       |         ${CMAKE_C_FLAGS_RELWITHDEBINFO}\n\
       `-- C++ flags\n\
           |-- Debug:\n\
           |     ${CMAKE_CXX_FLAGS_DEBUG}\n\
           |-- Release:\n\
           |     ${CMAKE_CXX_FLAGS_RELEASE}\n\
           `-- RelWithDebInfo (Release with debug info):\n\
                ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}\n"
)

message(STATUS "${MSG_CMAKE_SUMMARY}")
