#
# Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)
project(Boost.DI CXX)
set(CXX_STANDARD "c++14")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
        ${PROJECT_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/extension/include
)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fno-exceptions -pedantic -pedantic-errors -Wall -Wextra -Werror")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fno-exceptions -pedantic -pedantic-errors -Wall -Wextra -Werror")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(MSVC_STD_FLAG "/std:c++14" CACHE STRING "Language Standard Version. /std:[c++14|c++17|c++latest]  ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_STD_FLAG} /W3 /EHsc")
else()
    message(WARNING "Unsupported compiler!")
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else ()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif ()

option(BOOST_DI_OPT_BUILD_TESTS "Build and perform Boost.DI tests" ${IS_TOPLEVEL_PROJECT})
if (BOOST_DI_OPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(extension/test)
    add_subdirectory(test)
endif ()

option(BOOST_DI_OPT_BUILD_EXAMPLES "Build perform Boost.DI examples" ${IS_TOPLEVEL_PROJECT})
if (BOOST_DI_OPT_BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()
