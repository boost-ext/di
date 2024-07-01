#
# Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 3.13)
project(Boost.DI LANGUAGES CXX)

if(CMAKE_VERSION VERSION_LESS 3.21)
    # Emulates CMake 3.21+ behavior.
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        set(PROJECT_IS_TOP_LEVEL ON)
        set(${PROJECT_NAME}_IS_TOP_LEVEL ON)
    else()
        set(PROJECT_IS_TOP_LEVEL OFF)
        set(${PROJECT_NAME}_IS_TOP_LEVEL OFF)
    endif()
endif()

include(GNUInstallDirs)

find_package(Boost REQUIRED COMPONENTS headers)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
elseif(CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR "Boost.DI requires at least C++14")
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extension/include>
)
target_link_libraries(${PROJECT_NAME} INTERFACE Boost::headers)

option(BOOST_DI_OPT_BUILD_TESTS "Build and perform Boost.DI tests" ${PROJECT_IS_TOP_LEVEL})

if(BOOST_DI_OPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(extension/test)
    add_subdirectory(test)
endif()

option(BOOST_DI_OPT_BUILD_EXAMPLES "Build perform Boost.DI examples" ${PROJECT_IS_TOP_LEVEL})

if(BOOST_DI_OPT_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

option(BOOST_DI_OPT_INSTALL "Install Boost.DI" "NOT ${PROJECT_IS_TOP_LEVEL}")

if(BOOST_DI_OPT_INSTALL)
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Config
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        NAMESPACE Boost::
    )

    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()
