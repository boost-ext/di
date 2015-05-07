#
# Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)
project(Boost.DI CXX)
set(CXX_STANDARD "c++14")
enable_testing()
include_directories(include)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        #message(FATAL_ERROR "GCC version must be at least 5.0!")
    #endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pedantic -pedantic-errors -Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
        #message(FATAL_ERROR "Clang version must be at least 3.4!")
    #endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pedantic -pedantic-errors -Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0)
        #message(FATAL_ERROR "MSVC version must be at least 19.00!")
    #endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /EHsc")
else()
    message(WARNING "Unsupported compiler!")
endif()

if (BIICODE)
    include_directories(test/common)
    include_directories(test)

    ADD_BII_TARGETS()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include test.hpp")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include test.hpp")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI test.hpp")
    endif()

    target_compile_options(${BII_LIB_TARGET} INTERFACE ${CXX_EXTRA_FLAGS})
    return()
endif()

add_subdirectory(example)
add_subdirectory(test)

