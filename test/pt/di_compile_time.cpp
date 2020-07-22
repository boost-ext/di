//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(COVERAGE)

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <regex>
#include <sstream>
#include "common/utils.hpp"

namespace {

class generator {
  static constexpr auto SOURCE_FILE = "benchmark";
  static constexpr auto MAX_CTOR_ARGS = 10;
  static constexpr auto MAX_TYPES = 100;
  static constexpr auto MAX_COMPLEX_TYPES = 10;
  static constexpr auto MAX_MODULES = 10;

 public:
  enum class config_create { CTOR, INJECT };
  enum class config_configure { ALL, EXPOSED };

  generator(config_create create, config_configure configure, bool interfaces = false, int modules = 0)
      : create_(create),
        configure_(configure),
        interfaces_(interfaces),
        modules_(modules),
        source_code_(gen_name(create, configure, interfaces, modules)) {}

  std::string generate() {
    gen_header();
    gen_basic_types("x", "x");
    if (interfaces_) {
      gen_basic_types("i", "", "i");      // interfaces
      gen_basic_types("impl", "x", "i");  // implementations
    }
    gen_complex_types();
    gen_modules();
    gen_main();

    return source_code_;
  }

 private:
  std::string gen_name(config_create create, config_configure configure, bool interfaces, int modules) const {
    std::stringstream name;
    name << SOURCE_FILE << "_" << static_cast<int>(create) << "_" << static_cast<int>(configure) << "_" << interfaces << "_"
         << modules << ".cpp";
    return name.str();
  }

  void gen_header() {
    if (interfaces_) {
      source_code_ << "#include <memory>\n";
    }
    source_code_ << "#include <boost/di.hpp>\n";
    source_code_ << "namespace di = boost::di;\n";
  }

  auto gen_derived(const std::string& name, const std::string& base, int i) {
    std::stringstream result;
    if (base != "" && name != base) {
      result << " : " << base << std::setfill('0') << std::setw(2) << i;
    }
    return result.str();
  };

  auto gen_interface(const std::string& name, const std::string& base, int i) {
    std::stringstream result;
    if (name == base) {
      result << " virtual ~" << name << std::setfill('0') << std::setw(2) << i
             << "() noexcept = default; virtual void dummy() = 0;";
    }
    return result.str();
  };

  auto gen_implementation(const std::string& name, const std::string& base) {
    std::stringstream result;
    if (base != "" && name != base) {
      result << " void dummy() override { }";
    }
    return result.str();
  };

  auto gen_ctor_args(const std::string& constructor, int i) {
    std::stringstream args;
    if (constructor == "") {
      return args.str();
    }
    for (auto j = i - MAX_CTOR_ARGS; j < i; ++j) {
      if (j >= 0) {
        args << (j > 0 && j != i - MAX_CTOR_ARGS ? "," : "") << constructor << std::setfill('0') << std::setw(2) << j;
      }
    }
    return args.str();
  };

  void gen_basic_types(const std::string& name, const std::string& constructor, const std::string& base = "") {
    for (auto i = 0; i < MAX_TYPES; ++i) {
      std::stringstream ctor;

      if (create_ == config_create::INJECT) {
        ctor << "BOOST_DI_INJECT(" << name << std::setfill('0') << std::setw(2) << i << (i ? ", " : "")
             << gen_ctor_args(constructor, i) << ") { }";
      } else {
        ctor << name << std::setfill('0') << std::setw(2) << i << "(" << gen_ctor_args(constructor, i) << ") { }";
      }

      source_code_ << "struct " << name << std::setfill('0') << std::setw(2) << i << gen_derived(name, base, i) << " { "
                   << ctor.str() << gen_interface(name, base, i) << gen_implementation(name, base) << " };\n";
    }
  }

  void gen_complex_types() {
    for (auto i = 0; i < MAX_COMPLEX_TYPES; ++i) {
      source_code_ << "struct c" << i << "{";
      if (create_ == config_create::INJECT) {
        source_code_ << "BOOST_DI_INJECT(c" << i;
      } else {
        source_code_ << "c" << i << "(";
      }

      for (auto j = 0; j < MAX_COMPLEX_TYPES; ++j) {
        source_code_ << (j || create_ == config_create::INJECT ? ", " : "") << (interfaces_ ? "std::unique_ptr<" : "")
                     << (interfaces_ ? ((i * MAX_COMPLEX_TYPES) + j < modules_ * MAX_COMPLEX_TYPES ? "i" : "impl") : "x")
                     << std::setfill('0') << std::setw(2) << (i * MAX_COMPLEX_TYPES) + j << (interfaces_ ? ">" : "");
      }
      source_code_ << ") { } };\n";
    }

    if (create_ == config_create::INJECT) {
      source_code_ << "struct c { BOOST_DI_INJECT(c, ";
    } else {
      source_code_ << "struct c { c(";
    }

    for (auto i = 0; i < MAX_COMPLEX_TYPES; ++i) {
      source_code_ << (i ? ", " : "") << "c" << i;
    }
    source_code_ << ") { } };\n";
  }

  void gen_modules() {
    for (auto i = 0; i < MAX_MODULES; ++i) {
      if (i < modules_) {
        if (configure_ == config_configure::EXPOSED) {
          source_code_ << "di::injector<c" << i << "> ";
        } else {
          source_code_ << "auto ";
        }
        source_code_ << "module" << i << "() noexcept {\n";
        source_code_ << "\tauto i = di::make_injector(\n";

        if (interfaces_) {
          for (auto j = 0; j < MAX_MODULES; ++j) {
            source_code_ << "\t" << (j ? ", " : "  ") << "di::bind<i" << std::setfill('0') << std::setw(2)
                         << j + (i * MAX_MODULES) << ">().to<impl" << std::setfill('0') << std::setw(2) << j + (i * MAX_MODULES)
                         << ">()\n";
          }
        }

        for (auto j = 0; j < MAX_MODULES; ++j) {
          source_code_ << "\t\t" << (j || interfaces_ ? ", " : "  ") << "di::bind<x" << std::setfill('0') << std::setw(2)
                       << j + (i * MAX_MODULES) << ">().in(di::unique)\n";
        }
        source_code_ << "\t);\n";
        source_code_ << "\tusing module_t = decltype(i);\n";
        source_code_ << "\tstruct module : module_t { module(module_t&& object) : "
                        "module_t(static_cast<module_t&&>(object)){} };\n";
        source_code_ << "\treturn module{static_cast<module_t&&>(i)};\n";
        source_code_ << "\n};\n";
      }
    }
  }

  void gen_main() {
    source_code_ << "int main() {\n";
    source_code_ << "\tauto injector = di::make_injector(\n";
    for (auto i = 0; i < MAX_MODULES; ++i) {
      if (i < modules_) {
        source_code_ << "\t\t" << (i ? ", " : "  ") << "module" << i << "()\n";
      }
    }
    source_code_ << "\t);\n\tinjector.create<c>();\n}\n\n";
  }

 private:
  config_create create_;
  config_configure configure_;
  bool interfaces_ = false;
  int modules_ = 0;
  file<> source_code_;
};

class json {
  static constexpr auto header = R"(
{
  "title": {
    "text": "%title%"
  },
  "xAxis": {
    "title": {
      "text": "Number of bindings"
    },
    "floor": 0
  },

  "yAxis": {
    "title": {
      "text": "Time [s]"
    },
    "floor": 0
  },
  "series": [
)";

  static constexpr auto footer = R"(
  ]
}
)";

 public:
  explicit json(const std::string& name) : file_{name + "_" + cxx() + ".json"} {
    file_ << std::regex_replace(std::string{header}, std::regex{"%title%"}, name + " complexity | " + cxx() + " " + cxxflags());
  }

  ~json() { file_ << footer; }

  void series_start(const std::string& name) {
    data_count_ = 0;
    if (series_count_++) {
      file_ << ",\n";
    }
    file_ << "    {\n"
          << "      \"name\": "
          << "\"" << name << "\",\n      \"data\": [";
  }

  void series_end() { file_ << "]\n    }"; }

  void data(int n, double v) {
    if (data_count_++) {
      file_ << ", ";
    }
    file_ << "[" << n << ", " << v << "]";
  }

 private:
  int data_count_ = 0;
  int series_count_ = 0;
  file<> file_;
};

class series {
 public:
  series(json& data, const std::string& name) : data_(data) { data_.series_start(name); }
  ~series() { data_.series_end(); }

 private:
  json& data_;
};

auto measure(const std::string& file) {
  std::stringstream command;
  command << cxx() << " " << cxxflags(true) << " " << file;

  auto start = std::chrono::high_resolution_clock::now();
  auto result = std::system(command.str().c_str());
  auto end = std::chrono::high_resolution_clock::now();
  expect(!result);

  return std::chrono::duration<double>(end - start).count();
}

auto benchmark = [](const std::string& complexity, bool interfaces = false, int modules_min = 0, int modules_max = 10) {
  auto perform = [&](auto& ds, auto name, auto create, auto configure) {
    auto _ = series{ds, name};
    for (auto i = modules_min; i <= modules_max; ++i) {
      auto file = generator{create, configure, interfaces, i}.generate();
      ds.data(i * 10 * (interfaces ? 2 : 1), measure(file));
    }
  };

  json ds{complexity};
  perform(ds, "ctor/auto", generator::config_create::CTOR, generator::config_configure::ALL);
  perform(ds, "inject/all", generator::config_create::INJECT, generator::config_configure::ALL);
  perform(ds, "ctor/exposed", generator::config_create::CTOR, generator::config_configure::EXPOSED);
  perform(ds, "inject/exposed", generator::config_create::INJECT, generator::config_configure::EXPOSED);
};

auto is_benchmark(const std::string& name) { return std::getenv("BENCHMARK") && std::string{std::getenv("BENCHMARK")} == name; }
}  // namespace

test small_complexity = [] {
  if (is_benchmark("ON")) {
    benchmark("small", false /*interfaces*/, 0 /*min modules*/, 10 /*max modules*/);
  }
};

test big_complexity = [] {
  if (is_benchmark("ON")) {
    benchmark("big", true /*interfaces*/, 0 /*min modules*/, 10 /*max modules*/);
  }
};

test quick = [] {
  if (is_benchmark("QUICK")) {
    benchmark("quick", false /*interfaces*/, 1 /*min modules*/, 1 /*max modules*/);
  }
};

#endif
