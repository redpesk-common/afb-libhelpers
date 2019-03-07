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
set(AGLVERSION 6.99 CACHE STRING "Current AGL Version")
set(PROJECT_VERSION ${AGLVERSION} CACHE STRING "Project version can override AGLVERSION")
set(PROJECT_PRETTY_NAME "Afb Helpers")
set(PROJECT_DESCRIPTION "Library providing some helpful functions for apps using Application Framework Binder")
set(PROJECT_URL "https://gerrit.automotivelinux.org/gerrit/#/admin/projects/src/libafb-helpers")
set(PROJECT_AUTHOR "Ar Foll, Fulup")
set(PROJECT_AUTHOR_MAIL "fulup@iot.bzh")
set(PROJECT_LICENSE "APL2.0")
set(PROJECT_LANGUAGES "C")

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "/usr")
endif()

add_definitions(-DAFB_BINDING_VERSION=3)
ADD_COMPILE_OPTIONS(-DCONTROL_SUPPORT_LUA -Wall -Wextra -Wconversion -Wno-unused-parameter -Wno-sign-compare -Wno-sign-conversion -Werror=maybe-uninitialized -Werror=implicit-function-declaration -ffunction-sections -fdata-sections -fPIC)

option(AFB_HELPERS "Enable the basics helpers functions for the Application Framework Binders" ON)
option(AFB_HELPERS_QT "Enable the Qt helpers functions for the Application Framework Binders" OFF)

INCLUDE(FindPkgConfig)
set (PKG_REQUIRED_LIST
    afb-daemon
)

find_package(CURL REQUIRED)

# Loop on required package and add options
foreach (PKG_CONFIG ${PKG_REQUIRED_LIST})
    string(REGEX REPLACE "[<>]?=.*$" "" XPREFIX ${PKG_CONFIG})
    PKG_CHECK_MODULES(${XPREFIX} REQUIRED ${PKG_CONFIG})

    INCLUDE_DIRECTORIES(${${XPREFIX}_INCLUDE_DIRS})
    list(APPEND link_libraries ${${XPREFIX}_LDFLAGS})
    add_compile_options (${${XPREFIX}_CFLAGS})
endforeach(PKG_CONFIG)

if(${AFB_HELPERS})
    set(AFB_HELPERS_SRCS wrap-json.c filescan-utils.c escape.c afb-timer.c curl-wrap.c)

    set(AFB_HELPERS_HEADERS afb-helpers-utils.h curl-wrap.h escape.h
        filescan-utils.h wrap-json.h afb-timer.h
    )

    # Define targets
    ADD_LIBRARY(${TARGET_NAME} STATIC ${AFB_HELPERS_SRCS})

    target_link_libraries(${TARGET_NAME} curl)

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
endif()

if(${AFB_HELPERS_QT})
    message(STATUS "Qt's support enabled!")
    # Add target to project dependency list
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(Qt5WebSockets REQUIRED)

    set(AFB_HELPERS_QT_HEADERS qafbwebsocketclient.h)

    set(AFB_HELPERS_QT_SRCS qafbwebsocketclient.cpp)

    # Define Qt targets
    ADD_LIBRARY(${TARGET_NAME}-qt STATIC ${AFB_HELPERS_QT_SRCS})

    include_directories(${Qt5WebSockets_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME}-qt Qt5::WebSockets)

    # Define Qt target includes
    TARGET_INCLUDE_DIRECTORIES(${TARGET_NAME}-qt
            PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
        )

    CONFIGURE_FILE(afb-helpers-qt.pc.in afb-helpers-qt.pc @ONLY)
    INSTALL(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers-qt.pc
            DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/pkgconfig
           )
    INSTALL(TARGETS ${TARGET_NAME}-qt
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
           )
    INSTALL(FILES ${AFB_HELPERS_QT_HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
           )
endif()
