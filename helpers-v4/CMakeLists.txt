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

set(V4VERSION   4.0)
set(V4SOVERSION 4)

# Define target includes
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${AFB_INCLUDE_DIRS})
add_link_options(${AFB_LDFLAGS})
link_libraries(${AFB_LINK_LIBRARIES})
add_compile_options(${AFB_CFLAGS})


# Define targets
pkg_check_modules(DEPv4 REQUIRED json-c librp-utils)

add_library(afb-helpers4 SHARED
    afb-data-utils.c
    afb-req-utils.c
    ctl-lib.c
    plugin-store.c
)  

target_include_directories(afb-helpers4 PRIVATE ${DEPv4_INCLUDE_DIRS})
target_compile_options(afb-helpers4 PRIVATE ${DEPv4_CFLAGS} -fPIC)
target_compile_definitions(afb-helpers4 PUBLIC -DAFB_BINDING_VERSION=4)
target_link_options(afb-helpers4 PRIVATE ${DEPv4_LDFLAGS})
target_link_libraries(afb-helpers4 PUBLIC ${DEPv4_LINK_LIBRARIES} ${CMAKE_DL_LIBS})
set_target_properties(afb-helpers4 PROPERTIES VERSION ${V4VERSION} SOVERSION ${V4SOVERSION})

INSTALL(TARGETS afb-helpers4 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(FILES 
        afb-data-utils
        afb-data-utils.h
        afb-req-utils.h
        ctl-lib.h
        ctl-lib-plugin.h
        ctlplug.h
        plugin-store.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/afb-helpers4)

CONFIGURE_FILE(afb-helpers4.pc.in afb-helpers4.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers4.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
add_library(afb-helpers4-static STATIC
    afb-data-utils.c
    afb-req-utils.c
    ctl-lib.c
    plugin-store.c
)  

target_include_directories(afb-helpers4-static PRIVATE ${DEPv4_INCLUDE_DIRS})
target_compile_options(afb-helpers4-static PRIVATE ${DEPv4_CFLAGS} -fPIC)
target_compile_definitions(afb-helpers4-static PUBLIC -DAFB_BINDING_VERSION=4)
target_link_options(afb-helpers4-static PRIVATE ${DEPv4_LDFLAGS})
target_link_libraries(afb-helpers4-static PUBLIC ${DEPv4_LINK_LIBRARIES} ${CMAKE_DL_LIBS})
set_target_properties(afb-helpers4-static PROPERTIES OUTPUT_NAME afb-helpers4)

INSTALL(TARGETS afb-helpers4-static LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
CONFIGURE_FILE(afb-helpers4-static.pc.in afb-helpers4-static.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers4-static.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

add_library(afb-helpers++4 SHARED
    afb-data-utils.cpp
)
target_include_directories(afb-helpers++4 PRIVATE ${DEPv4_INCLUDE_DIRS})
target_compile_options(afb-helpers++4 PRIVATE ${DEPv4_CFLAGS} -fPIC)
target_compile_definitions(afb-helpers++4 PUBLIC -DAFB_BINDING_VERSION=4)
target_link_options(afb-helpers++4 PRIVATE ${DEPv4_LDFLAGS})
target_link_libraries(afb-helpers++4 PUBLIC ${DEPv4_LINK_LIBRARIES} afb-helpers4)
set_target_properties(afb-helpers++4 PROPERTIES VERSION ${V4VERSION} SOVERSION ${V4SOVERSION})

INSTALL(TARGETS afb-helpers++4 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(FILES
        afb-data-utils
        afb-data-utils.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/afb-helpers4)
CONFIGURE_FILE(afb-helpers++4.pc.in afb-helpers++4.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-helpers++4.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
