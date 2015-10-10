//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cstdlib>
#include <string>
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

struct interface { virtual ~interface() noexcept = default; virtual void dummy() = 0;};
struct implementation : interface { void dummy() override { } };

auto module = []() -> di::injector<interface> {
    return di::make_injector(
        di::bind<interface>().to<implementation>()
    );
};

auto test() {
    auto injector = di::make_injector(module());
    return injector.create<std::unique_ptr<interface>>();
}

int main(int, char** argv) {
    std::system(("gdb -batch -ex 'file " + std::string{argv[0]} + "' -ex 'disassemble test'").c_str());
}

