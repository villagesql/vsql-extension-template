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

cmake_minimum_required(VERSION 3.16)
project(vsql_extension_template)

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
