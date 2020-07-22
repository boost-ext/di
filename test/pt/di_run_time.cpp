//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(COVERAGE)

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

#define BOOST_DI_DISABLE_SHARED_PTR_DEDUCTION
#include "boost/di.hpp"
#include "common/utils.hpp"

namespace {

inline auto disassemble(const std::string& f, const std::regex& rgx) {
  file<> commands{f + ".cmd"};
  std::stringstream command;

#if defined(__linux)
  command << "gdb --batch -x " << commands;
  commands << "file " << get_module_file_name() << std::endl;
  commands << "disassemble " << f << std::endl;
  commands << "q" << std::endl;
#elif defined(__APPLE__)
  command << "lldb -s " << commands;
  commands << "file " << get_module_file_name() << std::endl;
  commands << "di -n " << f << std::endl;
  commands << "q" << std::endl;
#elif defined(_WIN32) || defined(_WIN64)
  command << "cdb -cf " << commands << " -z " << get_module_file_name();
  commands << "uf " << f << std::endl;
  commands << "q" << std::endl;
#endif

  std::stringstream result;
  if (std::system((command.str() + " > " + f + ".out").c_str())) {
    return result.str();
  }

  file<std::ifstream> output{f + ".out"};
  auto is_asm = false;
  for (std::string line; std::getline(output, line);) {
    if (std::regex_match(line, std::regex{".*:$"})) {
      is_asm = true;
    }

    if (is_asm) {
      std::smatch match;
      if (std::regex_search(line, match, rgx)) {
        result << match[1] << std::endl;
      }
    }
  }

  expect(!result.str().empty());
  return result.str();
}

inline auto check_opcodes(const std::string& name, const std::regex& rgx = std::regex{".*:(.*)"}) {
  const auto given = disassemble("given_" + name, rgx);
  const auto expected = disassemble("expected_" + name, rgx);
  const auto result = given == expected;
  if (!result) {
    std::printf("[expected]%s\n[given]%s\n", expected.c_str(), given.c_str());
  }
  return result;
}

}  // namespace

namespace di = boost::di;

struct i {
  virtual ~i() noexcept = default;
  virtual int dummy() = 0;
};
struct impl : i {
  explicit impl(int i) : i_(i){};
  int dummy() override { return i_; };
  int i_ = 0;
};

// ---------------------------------------------------------------------------

auto given_no_bindings() {
  auto injector = di::make_injector();
  return injector.create<int>();
}

auto expected_no_bindings() { return 0; }

test no_bindings = [] { expect(check_opcodes("no_bindings")); };

// ---------------------------------------------------------------------------

auto given_bind_int() {
  auto injector = di::make_injector(di::bind<int>().to(42));

  return injector.create<int>();
}

auto expected_bind_int() { return 42; }

test bind_int = [] { expect(check_opcodes("bind_int")); };

// ---------------------------------------------------------------------------

auto given_bind_interface() {
  auto injector = di::make_injector(di::bind<i>().to<impl>());

  return injector.create<std::unique_ptr<i>>();
}

auto expected_bind_interface() { return std::make_unique<impl>(0); }

test bind_interface = [] { expect(check_opcodes("bind_interface", std::regex{".*:([^ ]*).*"})); };

// ---------------------------------------------------------------------------

auto name_int = [] {};

struct c {
  BOOST_DI_INJECT(explicit c, (named = name_int) int);
};

c::c(int) {}

auto given_bind_named_int() {
  auto injector = di::make_injector(di::bind<int>().named(name_int).to(42));

  return injector.create<c>();
}

auto expected_bind_named_int() { return c{42}; }

test bind_named_int = [] { expect(check_opcodes("bind_named_int")); };

// ---------------------------------------------------------------------------

auto given_module_no_bindings() {
  auto module = [] { return di::make_injector(); };

  return di::make_injector(module()).create<int>();
}

auto expected_module_no_bindings() { return 0; }

test module_no_bindings = [] { expect(check_opcodes("module_no_bindings")); };

// ---------------------------------------------------------------------------

auto given_lambda_module_no_bindings() {
  auto module = [] { return di::make_injector(); };
  return di::make_injector(module()).create<int>();
}

auto expected_lambda_module_no_bindings() { return 0; }

test lambda_module_no_bindings = [] { expect(check_opcodes("lambda_module_no_bindings")); };

// ---------------------------------------------------------------------------

auto given_module_bind_int() {
  auto module = [] { return di::make_injector(di::bind<int>().to(42)); };

  return di::make_injector(module()).create<int>();
}

auto expected_module_bind_int() { return 42; }

test module_bind_int = [] { expect(check_opcodes("module_bind_int")); };

// ---------------------------------------------------------------------------

#endif
