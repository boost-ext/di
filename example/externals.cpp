//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <boost/di.hpp>
#include <boost/units/detail/utility.hpp>

namespace di = boost::di;

namespace {

struct c
{
    BOOST_DI_CTOR(c, int i) {
        std::cout << "i:= " << i << std::endl;
        assert(i == 32);
    }
};

} // namespace

int main()
{
    {
    using module_t = di::generic_module<
        di::external<int>
    >;

    module_t module(module_t::set<int>(32));
    module.create<c>();
    }

    {
    auto module =  di::fusion_module<>()(
        di::bind<int>::to(32)
    );

    using m = decltype(module);
    std::cout << boost::units::detail::demangle(typeid(m::deps::type).name()) << std::endl;
    module.create<c>();
    }

}

