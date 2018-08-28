###########################################################################
# Copyright 2015, 2016, 2017 IoT.bzh
#
# author: Fulup Ar Foll <rfulup@iot.bzh>
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

CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
include(GNUInstallDirs)
set(TARGET_NAME "afb-helpers")
set(PROJECT_PRETTY_NAME "Afb Helpers")
set(PROJECT_DESCRIPTION "afb helpers")
set(PROJECT_URL "https://github.com/iotbzh/4a-softmixer")
set(PROJECT_URL "https://gerrit.automotivelinux.org:29418/apps/app-afb-helpers-submodule.git")
set(PROJECT_AUTHOR "Ar Foll, Fulup")
set(PROJECT_AUTHOR_MAIL "fulup@iot.bzh")
set(PROJECT_LICENSE "APL2.0")
set(PROJECT_LANGUAGES "C")

# Add target to project dependency list
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "/usr")
endif()
add_definitions(-DAFB_BINDING_VERSION=3)

INCLUDE(FindPkgConfig)
set (PKG_REQUIRED_LIST
    afb-daemon
    )

# Loop on required package and add options
foreach (PKG_CONFIG ${PKG_REQUIRED_LIST})
    string(REGEX REPLACE "[<>]?=.*$" "" XPREFIX ${PKG_CONFIG})
    PKG_CHECK_MODULES(${XPREFIX} REQUIRED ${PKG_CONFIG})

    INCLUDE_DIRECTORIES(${${XPREFIX}_INCLUDE_DIRS})
    list(APPEND link_libraries ${${XPREFIX}_LDFLAGS})
    add_compile_options (${${XPREFIX}_CFLAGS})
endforeach(PKG_CONFIG)

set(AFB_HELPERS_SRCS wrap-json.c filescan-utils.c escape.c afb-timer.c)

option(AFB_HELPERS_QTWSCLIENT "Enable the Qt's websocket client to Application Framework Binders" ON)
find_package(CURL REQUIRED)

set(AFB_HELPERS_HEADERS afb-helpers-utils.h curl-wrap.h escape.h
    filescan-utils.h qafbwebsocketclient.h wrap-json.h afb-timer.h
    )

set(AFB_HELPERS_SRCS ${AFB_HELPERS_SRCS} qafbwebsocketclient.cpp qafbwebsocketclient.h)
find_package(Qt5WebSockets REQUIRED)
include_directories(${Qt5WebSockets_INCLUDE_DIRS})

set(AFB_HELPERS_SRCS ${AFB_HELPERS_SRCS} curl-wrap.c)

# Define targets
ADD_LIBRARY(${TARGET_NAME} STATIC ${AFB_HELPERS_SRCS})

target_link_libraries(${TARGET_NAME} Qt5::WebSockets)

target_link_libraries(${TARGET_NAME} curl)

# Library properties
SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME ${TARGET_NAME}
    )

# Define target includes
TARGET_INCLUDE_DIRECTORIES(${TARGET_NAME}
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    )

CONFIGURE_FILE(afb-helpers.pc.in afb-helpers.pc @ONLY)
INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers.pc
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )

INSTALL(TARGETS ${TARGET_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
INSTALL(FILES ${AFB_HELPERS_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
