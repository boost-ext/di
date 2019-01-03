//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_UTILS_HPP
#define BOOST_DI_UTILS_HPP

#include <limits.h>
#include <cstdlib>
#include <fstream>
#include <string>
#if defined(__linux)
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include "boost/di/aux_/compiler.hpp"

template <class TFStream = std::ofstream>
struct file : std::string, TFStream {
  file(const std::string& name)  // non explicit
      : std::string{name}, TFStream{name} {}
};

std::string cxx() {
  if (auto cpp = std::getenv("CXX")) {
    return cpp;
  }

#if defined(__CLANG__)
  return "clang++";
#elif defined(__GCC__)
  return "g++";
#elif defined(__MSVC__)
#if defined(__clang__)
  return "clang-cl";
#else
  return "cl";
#endif
#endif
}

std::string cxxflags(bool internal = false) {
  std::string cppflags;

  if (internal) {
    cppflags += "-I../include -I../../include ";  // bjam, cmake

#if defined(__CLANG__)
    cppflags += "-std=c++1y";
#elif defined(__GCC__)
    cppflags += "-std=c++1y";
#elif defined(__MSVC__) && _MSVC_LANG >= 201703L
    cppflags += "/std:c++17";
#elif defined(__MSVC__)
    cppflags += "";
#endif
  }

  if (auto flags = std::getenv("CXXFLAGS")) {
    cppflags += cppflags.empty() ? flags : std::string{" "} + flags;
  }

  return cppflags;
}

auto get_module_file_name() {
  char progname[FILENAME_MAX];

#if defined(__linux)
  auto len = 0;
  if ((len = readlink("/proc/self/exe", progname, sizeof(progname) - 1)) != -1) {
    progname[len] = '\0';
  }
#elif defined(__APPLE__)
  auto size = static_cast<unsigned int>(sizeof(progname));
  _NSGetExecutablePath(progname, &size);
#elif defined(_WIN32) || defined(_WIN64)
  GetModuleFileName(nullptr, progname, FILENAME_MAX);
#endif

  return std::string{progname};
}

#endif
