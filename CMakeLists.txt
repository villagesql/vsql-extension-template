# Copyright (c) 2026 VillageSQL Contributors
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.18)
project(vsql_extension_template)

# Default to an optimized build. An empty CMAKE_BUILD_TYPE yields -O0 (no
# -O flag applied), which builds and runs but leaves the code unoptimized.
# Override with -DCMAKE_BUILD_TYPE=.
#
# -DWITH_DEBUG=ON means "give me a debug build" -- it takes priority over
# any explicitly passed CMAKE_BUILD_TYPE and forces Debug, so NDEBUG is
# never defined (RelWithDebInfo/Release both define it, which would
# silently compile out every #ifndef NDEBUG check and every assert()).
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(WITH_DEBUG)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  elseif(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
  endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Extension name
set(EXTENSION_NAME "vsql_extension_template")

# Find VillageSQL SDK
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(VillageSQL REQUIRED)

message(STATUS "VillageSQL SDK found at: ${VillageSQL_PREFIX}")
message(STATUS "VillageSQL include dir: ${VillageSQL_INCLUDE_DIR}")

# Create the hello world shared library
add_library(hello SHARED
    src/hello.cc
)

# Include directories
target_include_directories(hello PRIVATE
    ${VillageSQLExtensionFramework_INCLUDE_DIR}
)

# Create the VEB package
VEF_CREATE_VEB(
    NAME ${EXTENSION_NAME}
    LIBRARY_TARGET hello
    MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/manifest.json
)
