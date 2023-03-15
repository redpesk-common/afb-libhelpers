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

# Define target includes
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${AFB_INCLUDE_DIRS})
add_link_options(${AFB_LDFLAGS})
link_libraries(${AFB_LINK_LIBRARIES})
add_compile_options(${AFB_CFLAGS})

set(AFB_HELPERS_SRCS_V3
    curl-wrap.c
    escape.c
    filescan-utils.c
    wrap-base64.c
    wrap-json.c
    afb-timer.c
)
set(AFB_HELPERS_HEADERS_V3
    curl-wrap.h
    escape.h
    filescan-utils.h
    json.hpp
    wrap-base64.h
    wrap-json.h
    afb-timer.h
)

find_package(CURL REQUIRED)
pkg_check_modules(DEPv3 REQUIRED json-c libsystemd)

ADD_LIBRARY(afb-helpers3 SHARED ${AFB_HELPERS_SRCS_V3})
target_include_directories(afb-helpers3 PRIVATE ${DEPv3_INCLUDE_DIRS})
target_compile_options(afb-helpers3 PRIVATE ${DEPv3_CFLAGS} -fPIC)
target_compile_definitions(afb-helpers3 PUBLIC -DAFB_BINDING_VERSION=3)
target_link_options(afb-helpers3 PRIVATE ${DEPv3_LDFLAGS})
target_link_libraries(afb-helpers3 PUBLIC ${DEPv3_LINK_LIBRARIES} curl)
set_target_properties(afb-helpers3 PROPERTIES OUTPUT_NAME afb-helpers SOVERSION 3)

INSTALL(TARGETS afb-helpers3 DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(FILES ${AFB_HELPERS_HEADERS_V3} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/afb-helpers)

CONFIGURE_FILE(afb-helpers3.pc.in afb-helpers3.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers3.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers3.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig RENAME afb-helpers.pc)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers3.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig RENAME afb-libhelpers.pc)
