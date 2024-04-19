###########################################################################
# Copyright 2015-2024 IoT.bzh Company
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
project(afb-libhelpers
    VERSION 10.0.7
    DESCRIPTION "Library providing some helpful functions for bindings of Application Framework Binder"
    HOMEPAGE_URL "https://github.com/redpesk-common/afb-libhelpers"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

include(GNUInstallDirs)

ADD_COMPILE_OPTIONS(
    -Wall
    -Wextra
    -Wconversion
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wno-sign-conversion
    -Werror=maybe-uninitialized
    $<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>
    -ffunction-sections
    -fdata-sections)

add_link_options(
    -Wl,--no-undefined)

option(AFB_HELPERS_V3 "Enable the basics helpers functions for the Application Framework Binders V3" ON)
option(AFB_HELPERS_V4 "Enable the basics helpers functions for the Application Framework Binders V4" ON)

INCLUDE(FindPkgConfig)


pkg_check_modules(AFB REQUIRED afb-binding)

# Define target includes
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${AFB_INCLUDE_DIRS})
add_link_options(${AFB_LDFLAGS})
link_libraries(${AFB_LINK_LIBRARIES})
add_compile_options(${AFB_CFLAGS})

if(${AFB_HELPERS_V3})
    add_subdirectory(helpers-v3)
endif()

if(${AFB_HELPERS_V4})
    add_subdirectory(helpers-v4)
endif()
