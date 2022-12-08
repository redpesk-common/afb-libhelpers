###########################################################################
# Copyright 2015, 2016, 2017 IoT.bzh
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
    VERSION 10.0.0
    DESCRIPTION "Library providing some helpful functions for apps using Application Framework Binder"
    HOMEPAGE_URL "https://github.com/redpesk-common/afb-libhelpers"
    LANGUAGES C CXX)

set(PROJECT_PRETTY_NAME "AFB Helpers")
set(PROJECT_AUTHOR "IoT.bzh")
set(PROJECT_AUTHOR_MAIL "fulup@iot.bzh")
set(PROJECT_LICENSE "APL2.0")
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
    -fdata-sections
    -fPIC)

add_link_options(
    -Wl,--no-undefined)

option(AFB_HELPERS_V3 "Enable the basics helpers functions for the Application Framework Binders V3" ON)
option(AFB_HELPERS_V4 "Enable the basics helpers functions for the Application Framework Binders V4" ON)
option(AFB_HELPERS_QT "Enable the Qt helpers functions for the Application Framework Binders" OFF)

INCLUDE(FindPkgConfig)
find_package(CURL REQUIRED)


pkg_check_modules(AFB REQUIRED afb-binding)

# Define target includes
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${AFB_INCLUDE_DIRS})
add_link_options(${AFB_LDFLAGS})
link_libraries(${AFB_LINK_LIBRARIES})
add_compile_options(${AFB_CFLAGS})

if(${AFB_HELPERS_V3})
    set(AFB_HELPERS_SRCS_V3
        curl-wrap.c
        escape.c
        filescan-utils.c
        wrap-base64.c
        wrap-json.c
        afb-timer.c
    )

    set(AFB_HELPERS_HEADERS_V3
        afb-helpers-utils.h
        curl-wrap.h
        escape.h
        filescan-utils.h
        json.hpp
        wrap-base64.h
        wrap-json.h
        afb-timer.h
    )

    pkg_check_modules(DEPv3 REQUIRED json-c libsystemd)

    ADD_LIBRARY(afb-helpers3 SHARED ${AFB_HELPERS_SRCS_V3})
    target_include_directories(afb-helpers3 PRIVATE ${DEPv3_INCLUDE_DIRS})
    target_compile_options(afb-helpers3 PRIVATE ${DEPv3_CFLAGS})
    target_compile_definitions(afb-helpers3 PUBLIC -DAFB_BINDING_VERSION=3)
    target_link_options(afb-helpers3 PRIVATE ${DEPv3_LDFLAGS})
    target_link_libraries(afb-helpers3 PUBLIC ${DEPv3_LINK_LIBRARIES} curl)
    set_target_properties(afb-helpers3 PROPERTIES OUTPUT_NAME afb-helpers SOVERSION 3)
    INSTALL(TARGETS afb-helpers3 DESTINATION ${CMAKE_INSTALL_LIBDIR})
    INSTALL(FILES ${AFB_HELPERS_HEADERS_V3} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/afb-helpers3)

    CONFIGURE_FILE(afb-helpers3.pc.in afb-helpers3.pc @ONLY)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers3.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers3.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig RENAME afb-helpers.pc)
endif()

if(${AFB_HELPERS_V4})

    set(AFB_HELPERS_HEADERS_V4
    )

    # Define targets

    pkg_check_modules(DEPv4 REQUIRED json-c)

    add_library(afb-helpers4 SHARED
        curl-wrap.c
        escape.c
        filescan-utils.c
        afb-data-utils.c
        afb-data-utils.cpp
        afb-plugin-helpers.c
    )  
    target_include_directories(afb-helpers4 PRIVATE ${DEPv4_INCLUDE_DIRS})
    target_compile_options(afb-helpers4 PRIVATE ${DEPv4_CFLAGS})
    target_compile_definitions(afb-helpers4 PUBLIC -DAFB_BINDING_VERSION=4)
    target_link_options(afb-helpers4 PRIVATE ${DEPv4_LDFLAGS})
    target_link_libraries(afb-helpers4 PUBLIC ${DEPv4_LINK_LIBRARIES} curl ${CMAKE_DL_LIBS})
    set_target_properties(afb-helpers4 PROPERTIES SOVERSION 4)
    INSTALL(TARGETS afb-helpers4 DESTINATION ${CMAKE_INSTALL_LIBDIR})
    INSTALL(FILES 
            afb-helpers-utils.h
            afb-data-utils
            afb-data-utils.h
            afb-data-utils.hpp
            afb-plugin-helpers.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/afb-helpers4)

    CONFIGURE_FILE(afb-helpers4.pc.in afb-helpers4.pc @ONLY)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers4.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()


if(${AFB_HELPERS_QT})
    message(STATUS "Qt's support enabled!")
    # Add target to project dependency list
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(Qt5WebSockets REQUIRED)

    # Define Qt targets
    ADD_LIBRARY(afb-helpers-qt SHARED qafbwebsocketclient.cpp)

    include_directories(${Qt5WebSockets_INCLUDE_DIRS})
    target_link_libraries(afb-helpers-qt Qt5::WebSockets)

    # Define Qt target includes
    TARGET_INCLUDE_DIRECTORIES(afb-helpers-qt
            PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
        )

    CONFIGURE_FILE(afb-libhelpers-qt.pc.in afb-helpers-qt.pc @ONLY)
    INSTALL(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers-qt.pc
            DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/pkgconfig
           )
    INSTALL(TARGETS afb-helpers-qt
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
           )
    INSTALL(FILES qafbwebsocketclient.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
           )
endif()
