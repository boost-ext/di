//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cstdlib>
#include <fstream>
#include <regex>
#include "boost/di.hpp"

#if !defined(COVERAGE)

namespace {

auto disassemble(const std::string& f, const std::string& progname, const std::regex& rgx) {
    std::stringstream result;
    std::string commands_file = std::tmpnam(nullptr);
    std::ofstream commands{commands_file};
    std::string output_file = std::tmpnam(nullptr);
    std::stringstream command;

#if defined(__linux)
    command << "gdb --batch -x " << commands_file;
    commands << "file " << progname << std::endl;
    commands << "disassemble " << f << std::endl;
    commands << "q" << std::endl;
#elif defined(__APPLE__)
    command << "lldb -s " << commands_file;
    commands << "file " << progname << std::endl;
    commands << "di -n " << f << std::endl;
    commands << "q" << std::endl;
#elif defined(_WIN32) || defined(_WIN64)
    command << "cdb -cf " << commands_file << " -z " << progname;
    commands << "uf " << f << std::endl;
    commands << "q" << std::endl;
#endif

    if (std::system((command.str()  + " > " + output_file).c_str())) {
        return result.str();
    }

    std::ifstream output{output_file};
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

bool check_opcodes(const char* progname, const std::string& name, const std::regex& rgx = std::regex{".*:(.*)"}) {
    return disassemble("given_" + name, progname, rgx) == disassemble("expected_" + name, progname, rgx);
}

} // namespace

namespace di = boost::di;

struct i { virtual ~i() noexcept = default; virtual int dummy() = 0; };
struct impl : i { impl(int i) : i_(i) { }; int dummy() override { return i_; }; int i_ = 0; };

// ---------------------------------------------------------------------------

auto given_no_bindings() {
    auto injector = di::make_injector();
    return injector.create<int>();
}

auto expected_no_bindings() {
    return 0;
}

test no_bindings = [](auto progname) {
    expect(check_opcodes(progname, "no_bindings"));
};

// ---------------------------------------------------------------------------

auto given_bind_int() {
    auto injector = di::make_injector(
        di::bind<int>().to(42)
    );

    return injector.create<int>();
}

auto expected_bind_int() {
    return 42;
}

test bind_int = [](auto progname) {
    expect(check_opcodes(progname, "bind_int"));
};

// ---------------------------------------------------------------------------

auto given_bind_interface() {
    auto injector = di::make_injector(
        di::bind<i, impl>()
    );

    return injector.create<std::unique_ptr<i>>();
}

auto expected_bind_interface() {
    return std::make_unique<impl>(0);
}

test bind_interface = [](auto progname) {
    expect(check_opcodes(progname, "bind_interface", std::regex{".*:([^ ]*).*"}));
};

// ---------------------------------------------------------------------------

auto name_int = []{};

struct c {
    BOOST_DI_INJECT(c, (named = name_int) int);
};

c::c(int) { }

auto given_bind_named_int() {
    auto injector = di::make_injector(
        di::bind<int>().named(name_int).to(42)
    );

    return injector.create<c>();
}

auto expected_bind_named_int() {
    return c{42};
}

test bind_named_int = [](auto progname) {
    expect(check_opcodes(progname, "bind_named_int"));
};

// ---------------------------------------------------------------------------

auto given_module_no_bindings() {
    struct module {
        auto configure() const noexcept {
            return di::make_injector();
        }
    };

    return di::make_injector(module{}).create<int>();
}

auto expected_module_no_bindings() {
    return 0;
}

test module_no_bindings = [](auto progname) {
    expect(check_opcodes(progname, "module_no_bindings"));
};

// ---------------------------------------------------------------------------

auto given_lambda_module_no_bindings() {
    auto module = [] {
        return di::make_injector();
    };
    return di::make_injector(module()).create<int>();
}

auto expected_lambda_module_no_bindings() {
    return 0;
}

test lambda_module_no_bindings = [](auto progname) {
    expect(check_opcodes(progname, "lambda_module_no_bindings"));
};

// ---------------------------------------------------------------------------

auto given_module_bind_int() {
    struct module {
        auto configure() const noexcept {
            return di::make_injector(
                di::bind<int>().to(42)
            );
        }
    };

    return di::make_injector(module{}).create<int>();
}

auto expected_module_bind_int() {
    return 42;
}

test module_bind_int = [](auto progname) {
    expect(check_opcodes(progname, "module_bind_int"));
};

// ---------------------------------------------------------------------------

auto given_lambda_module_bind_int() {
    auto module = [] {
        return di::make_injector(
            di::bind<int>().to(42)
        );
    };

    return di::make_injector(module()).create<int>();
}

auto expected_lambda_module_bind_int() {
    return 42;
}

test lambda_module_bind_int = [](auto progname) {
    expect(check_opcodes(progname, "lambda_module_bind_int"));
};

// ---------------------------------------------------------------------------

auto given_bind_interface_shared() {
    auto injector = di::make_injector(
        di::bind<i, impl>().in(di::shared)
    );

    return injector.create<std::shared_ptr<i>>();
}

auto expected_bind_interface_shared() {
    return std::make_shared<impl>(0);
}

#if 0
    test bind_interface_shared = [](auto progname) {
        expect(check_opcodes(progname, "bind_interface_shared", std::regex{".*:([^ ]*).*"}));
    };
#endif

// ---------------------------------------------------------------------------

#endif

