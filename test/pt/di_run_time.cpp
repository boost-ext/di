//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

#include <cstdio>
#include <regex>

#include <iostream>
namespace di = boost::di;

auto disassemble(const char* f, const char* progname) {
    FILE *in = nullptr;

    std::stringstream command;
    std::stringstream result;
    command << "gdb -batch -ex 'file ./" << progname << " ' -ex 'disassemble " << f << "'";

    if (!(in = popen(command.str().c_str(), "r"))) {
        return result.str();
    }

    char buff[512];
    while (fgets(buff, sizeof(buff), in)) {
        result << buff;
    }

    pclose(in);
    return result.str();
}

struct i { virtual ~i() noexcept = default; virtual int dummy() = 0; };
struct impl : i { impl(int i) : i_(i) { }; int dummy() override { return i_; }; int i_ = 0; };

auto test_no_bindings() {
    auto injector = di::make_injector();
    return injector.create<int>();
}

//test no_bindings = [](auto progname) {
    //std::cout << disassemble("test_no_bindings", progname) << std::endl;
    //expect_eq(
        //"blah"
      //, disassemble("test_no_bindings", progname)
    //);
//};

auto test_bind_int() {
    auto injector = di::make_injector(
        di::bind<int>.to(42)
    );

    return injector.create<int>();
}

auto expect_asm(const char* progname, const char* f, const std::vector<std::string>& args) {
    std::stringstream regex;
    regex << "^Dump.*:\\n";
    for (const auto& arg : args) {
        regex << ".*" << arg << ".*\\n";
    }
    regex << ".*dump\\\\.\\n";

    expect(std::regex_match(disassemble(f, progname), std::regex(regex.str())));
        //std::regex("^Dump.*:\n.*mov.*0x2a.*\n.*retq.*\n.*dump\\.\n")));
    std::cout << regex.str() << std::endl;
}

test bind_int = [](auto progname) {
    expect_asm(progname, "test_bind_int", 
        {
            "mov.*0x2a"
          , "retq"
        }
    );
};

//test_perf bind_int = [] { };

//auto test_bind_interface() {
    //auto injector = di::make_injector(
        //di::bind<int>.to(42)
      //, di::bind<i, impl>
    //);

    //auto ptr = injector.create<std::unique_ptr<i>>();
    //return ptr->dummy();
//}

//test_perf bind_interface = [] { };

//auto test_bind_interface_shared() {
    //auto injector = di::make_injector(
        //di::bind<int>.to(42)
      //, di::bind<i, impl>.in(di::singleton)
    //);

    //auto ptr = injector.create<std::shared_ptr<i>>();
    //return ptr->dummy();
//}

//test_perf bind_intterface_shared = [] { };

//struct module {
    //di::injector<i> configure() const noexcept {
        //return  di::make_injector(
            //di::bind<int>.to(42)
          //, di::bind<i, impl>
        //);
    //};
//};

//auto test_exposed() {
    //auto injector = di::make_injector(module{});
    //auto* ptr =  injector.create<i*>();
    //return ptr->dummy();
//}

//test_perf exposed = [] { };


