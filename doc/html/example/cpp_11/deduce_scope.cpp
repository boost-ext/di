//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[deduce_scope_cpp_11
//````C++11```
//<-
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace {
struct if0 { virtual ~if0() { } virtual void dummy() = 0; };
struct c0if0 : if0 { void dummy() override { } };
struct c2 { c2(std::shared_ptr<if0> /*shared*/) { } };
struct c3 { c3(std::shared_ptr<if0> /*shared*/) { } };
struct c4 { c4(std::unique_ptr<c2> /*unique*/, std::unique_ptr<c3> /*unique*/) { } };
} // namespace

int main() {
    {
        auto injector = di::make_injector(
            di::deduce<c0if0>()
        );

        injector.create<c4>();
    }

    {
        using injector = di::injector<
            c0if0
        >;

        injector().create<c4>();
    }

    return 0;
}

//`[table
//`[[Full code example: [@example/cpp_11/deduce_scope.cpp deduce_scope.cpp]]]]
//]

