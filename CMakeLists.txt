# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

# Require cmake that supports BYPRODUCTS in add_custom_command, ExternalProject_Add [1].
cmake_minimum_required(VERSION 3.2.0)

project(parquet-arrow-example)

include(ExternalProject)
include(FindPkgConfig)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

# This ensures that things like gnu++11 get passed correctly
set(CMAKE_CXX_STANDARD 11)

# We require a C++11 compliant compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# We want to link dynamically against Arrow and Parquet
set(PARQUET_BUILD_SHARED ON)


# First search the packages in the system. If they are not found, use CMake's
# ExternalProject mechanism to build them locally.
find_package(Arrow)
if (NOT ARROW_FOUND)
  # set compile output directory
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
  endif(NOT CMAKE_BUILD_TYPE)
  string (TOLOWER ${CMAKE_BUILD_TYPE} BUILD_SUBDIR_NAME)
  # If build in-source, create the latest symlink. If build out-of-source, which is
  # preferred, simply output the binaries in the build folder
  if (${CMAKE_SOURCE_DIR} STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    set(BUILD_OUTPUT_ROOT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build/${BUILD_SUBDIR_NAME}")
    # Link build/latest to the current build directory, to avoid developers
    # accidentally running the latest debug build when in fact they're building
    # release builds.
    FILE(MAKE_DIRECTORY ${BUILD_OUTPUT_ROOT_DIRECTORY})
    if (NOT APPLE)
      set(MORE_ARGS "-T")
    endif()
  EXECUTE_PROCESS(COMMAND ln ${MORE_ARGS} -sf ${BUILD_OUTPUT_ROOT_DIRECTORY}
    ${CMAKE_CURRENT_BINARY_DIR}/build/latest)
  else()
    set(BUILD_OUTPUT_ROOT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${BUILD_SUBDIR_NAME}")
  endif()

  include(ArrowExternalProject)
  set(ARROW_VENDORED 1)
else()
  set(ARROW_VENDORED 0)
endif()
find_package(Parquet)

include_directories(SYSTEM ${ARROW_INCLUDE_DIR} ${PARQUET_INCLUDE_DIR})

add_executable(parquet-arrow-reader-writer src/plugin/reader.cc)
target_link_libraries(parquet-arrow-reader-writer ${PARQUET_SHARED_LIB} ${ARROW_SHARED_LIB})
if (ARROW_VENDORED)
  add_dependencies(parquet-arrow-reader-writer arrow_ep)
endif()
