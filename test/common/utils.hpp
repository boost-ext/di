//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
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
#include <unordered_map>
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
    static std::unordered_map<std::string, std::string> flags = {
        {"msvc14", "/std:c++14"},     {"clang14", "-std=c++1y"}, {"gcc14", "-std=c++1y"},
        {"msvc17", "/std:c++17"},     {"clang17", "-std=c++1z"}, {"gcc17", "-std=c++1z"},
        {"msvc20", "/std:c++latest"}, {"clang20", "-std=c++2a"}, {"gcc20", "-std=c++2a"}};

    std::string compiler = "";
#if defined(__CLANG__)
    compiler = "clang";
#elif defined(__GCC__)
    compiler = "gcc";
#elif defined(__MSVC__)
    compiler = "msvc";
#endif

    std::string cxx_version = "";
#if __cplusplus <= 201402L
    cxx_version = "14";
#elif __cplusplus == 201703L
    cxx_version = "17";
#elif __cplusplus >= 201704L
    cxx_version = "20";
#endif
    cppflags += flags[compiler + cxx_version];
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
