//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct i1 { virtual ~i1() { } };
struct i2 { virtual ~i2() { } };
struct impl : i1, i2 { };

class multiple_interfaces
{
public:
    multiple_interfaces(const std::shared_ptr<i1>& i1_, const std::shared_ptr<i2>& i2_)
    {
        assert(dynamic_cast<impl*>(i1_.get()));
        assert(dynamic_cast<impl*>(i2_.get()));
        assert(static_cast<impl*>(i1_.get()) == static_cast<impl*>(i2_.get()));
    }
};

} // namespace

int main() {
    {
        di::injector<
            impl
        >().create<multiple_interfaces>();
    }

    {
        di::make_injector(
            di::deduce<impl>()
        ).create<multiple_interfaces>();
    }

    {
        di::make_injector(
            di::bind<di::any_of<i1, i2>, impl>()
        ).create<multiple_interfaces>();
    }

    return 0;
}

