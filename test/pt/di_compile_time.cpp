//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <chrono>

namespace {

template<class TFStream = std::ofstream>
struct file : std::string, TFStream {
    file(const std::string& name) // non explicit
        : std::string{name}, TFStream{name}
    { }
};

enum class config_create { CTOR, INJECT };
enum class config_configure { ALL, EXPOSED };

std::string compiler() {
    if (auto cpp = std::getenv("CXX")) {
        return cpp;
    }

    #if defined(__clang__)
        return "clang++";
    #elif defined(__GNUC__) && !defined(__clang__)
        return "g++";
    #elif defined(_MSC_VER)
        return "cl";
    #endif
}

std::map<std::string, std::string> complexities = {
    {"small_complexity", "x99" }
  , {"medium_complexity", "c0" }
  , {"big_complexity", "c0, c1, c2, c3, c4, c5, c6, c7, c8, c9" }
};

std::string generate(const std::string& complexity
                   , config_create create
                   , config_configure configure
                   , bool bindings = false
                   , bool interfaces = false
                   , int modules = 0) {
    file<> source_code{"benchmark.cpp"};
    source_code << "#include <memory>\n";
    source_code << "#include <boost/di.hpp>\n";
    source_code << "namespace di = boost::di;\n";

    auto gen_structs = [&](const std::string& name, const std::string& constructor, const std::string& base = "") {
        for (auto i = 0; i < 100; ++i) {
            std::stringstream ctor;

            auto extend = [&](auto i) {
                std::stringstream result;
                if (base != "" && name != base) {
                    result << " : " << base << std::setfill('0') << std::setw(2) << i;
                }
                return result.str();
            };

            auto interface = [&](auto i) {
                std::stringstream result;
                if (name == base) {
                    result << " virtual ~" << name << std::setfill('0') << std::setw(2) << i
                           << "() noexcept = default; virtual void dummy() = 0;";
                }
                return result.str();
            };

            auto implementation = [&]() {
                std::stringstream result;
                if (base != "" && name != base) {
                    result << " void dummy() override { }";
                }
                return result.str();
            };

            auto ctor_args = [&](auto i) {
                std::stringstream args;
                if (constructor == "") {
                    return args.str();
                }
                for (auto j = i - 10 ; j < i; ++j) {
                    if (j >= 0) {
                        args << (j > 0 && j != i-10 ? "," : "") << constructor << std::setfill('0') << std::setw(2) << j;
                    }
                }
                return args.str();
            };

            if (create == config_create::INJECT) {
                ctor << "BOOST_DI_INJECT(" << name << std::setfill('0') << std::setw(2) << i << (i ? ", " : "") << ctor_args(i) << ") { }";
            } else {
                ctor << name << std::setfill('0') << std::setw(2) << i << "(" << ctor_args(i) << ") { }";
            }

            source_code << "struct " << name << std::setfill('0') << std::setw(2) << i
                        << extend(i) << " { "
                        << ctor.str()
                        << interface(i)
                        << implementation()
                        << " };\n";
        }
    };

    gen_structs("x", "x");
    gen_structs("i", "", "i");
    gen_structs("impl", "x", "i");

    for (auto i = 0; i < 10; ++i) {
        source_code << "struct c" << i << "{";
        if (create == config_create::INJECT) {
            source_code << "BOOST_DI_INJECT(c" << i;
        } else {
            source_code << "c" << i << "(";
        }

        for (auto j = 0; j < 10; ++j) {
            source_code << (j || create == config_create::INJECT ? ", " : "")
                        << "std::unique_ptr<i" << std::setfill('0') << std::setw(2) << (i*10)+j << ">";
        }
        source_code << ") { } };\n";
    }

    for (auto i = 0; i < 10; ++i) {
        if (i < modules) {
            source_code << "struct module" << i << " {\n";
            if (configure == config_configure::EXPOSED) {
                source_code << "\tdi::injector<c" << i << "> ";
            } else {
                source_code << "\tauto ";
            }
            source_code << "configure() const noexcept {\n";
            source_code << "\t\treturn di::make_injector(\n";

            if (interfaces) {
                for (auto j = 0; j < 10; ++j) {
                    source_code << "\t\t" << (j ? ", " : "  ")
                                << "di::bind<i" << std::setfill('0') << std::setw(2) << j + (i*10)
                                << ", impl" << std::setfill('0') << std::setw(2) << j + (i * 10) << ">()\n";
                }
            }

            if (bindings) {
                for (auto j = 0; j < 10; ++j) {
                    source_code << "\t\t" << (j || interfaces ? ", " : "  ")
                                << "di::bind<x" << std::setfill('0') << std::setw(2) << j + (i*10)
                                << ">().in(di::unique)\n";
                }
            }

            source_code << "\t\t);\n\t}\n};\n";
        }
    }

    source_code << "struct " << complexity << " { ";
    if (create == config_create::INJECT) {
        source_code << "BOOST_DI_INJECT(" << complexity << ", ";
    } else {
        source_code << complexity << "(";
    }

    source_code << complexities[complexity];

    source_code << ") { } };\n";

    source_code << "int main() {\n";
    source_code << "\tauto injector = di::make_injector(\n";
    for (auto i = 0; i < 10; ++i) {
        if (i < modules) {
            source_code << "\t\t" << (i ? ", " : "  ") << "module" << i << "{}\n";
        }
    }
    source_code << "\t);\n}\n";

    return source_code;
}

auto measure(const std::string& file) {
    std::stringstream command;
    std::string cxx{compiler()};
    std::string cxxflags = "-I../include -I../../include ";

    #if defined(__clang__)
        cxxflags += "-std=c++1y";
    #elif defined(__GNUC__) && !defined(__clang__)
        cxxflags += "-std=c++1y";
    #elif defined(_MSC_VER)
        cxxflags += "";
    #endif

    command << cxx << " " << cxxflags << " " << file;

    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto result = std::system(command.str().c_str());
    auto end = high_resolution_clock::now();
    expect(!result);

    return duration<double>(end - start).count();
}

class json {
static constexpr auto header = R"(
{
  "title": {
    "text": "Small complexity | clang++ -O2"
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
    explicit json(const std::string& name) : file_{name} {
        file_ << header;
    }

    ~json() {
        file_ << footer;
    }

    void series_start(const std::string& name) {
        data_count_ = 0;
        if (series_count_++) {
            file_ << ",\n";
        }
        file_ << "    {\n" << "      \"name\": " << "\"" << name << "\",\n      \"data\": [";
    }

    void series_end() {
        file_ << "]\n    }";
    }

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
    series(json& data, const std::string& name) : data_(data) {
        data_.series_start(name);
    }

    ~series() {
        data_.series_end();
    }

private:
    json& data_;
};

}

auto benchmark = [](const std::string& complexity, int n = 10) {
    json ds{complexity + std::string{"_"} + compiler() + ".json"};

    {
        auto _ = series{ds, "ctor/auto"};
        for (auto i = 0; i <= n; ++i) {
            ds.data(i*20, measure(generate(complexity, config_create::CTOR, config_configure::ALL)));
        }
    }

    {
        auto _ = series{ds, "inject/all"};
        for (auto i = 0; i <= n; ++i) {
            ds.data(i*20, measure(generate(complexity, config_create::INJECT, config_configure::ALL)));
        }
    }

    {
        auto _ = series{ds, "ctor/exposed"};
        for (auto i = 0; i <= n; ++i) {
            ds.data(i*20, measure(generate(complexity, config_create::CTOR, config_configure::EXPOSED)));
        }
    }

    {
        auto _ = series{ds, "inject/exposed"};
        for (auto i = 0; i <= n; ++i) {
            ds.data(i*20, measure(generate(complexity, config_create::INJECT, config_configure::EXPOSED)));
        }
    }
};

test small = [] {
    generate("small_complexity", config_create::CTOR, config_configure::ALL, false, true, 10);
    //benchmark("small_complexity", 2);
};

test medium = [] {
    //benchmark("medium_complexity", bind_others, 2);
};

test big = [] {
    //benchmark("big_complexity", bind_all, 2);
};

test quick = [] {
};

