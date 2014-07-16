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
#include <boost/shared_ptr.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } virtual void dummy() = 0; };
struct impl : i { void dummy() override { } };

struct c2 {
    c2(std::shared_ptr<i> spi /*shared*/
     , const boost::shared_ptr<i>& spi_ /*shared*/)
        : spi_(spi), spi__(spi_)
    {
        assert(spi.get() == spi_.get());
    }

    std::shared_ptr<i> spi_;
    boost::shared_ptr<i> spi__;
};

struct c3 {
    c3(std::shared_ptr<i> spi/*shared*/
     , int i/*unique*/)
        : spi_(spi)
    {
        assert(i == 0);
    }

    std::shared_ptr<i> spi_;
};

struct c4 {
    c4(std::unique_ptr<c2> c2_ /*unique*/
     , const c3& c3_ /*unique temporary*/)
    {
        assert(c3_.spi_.get() == c2_->spi_.get());
        assert(c3_.spi_.get() == c2_->spi__.get());
    }
};

int main() {
    {
        auto injector = di::make_injector(
            di::deduce<impl>()
        );

        injector.create<c4>();
    }

    {
        using injector = di::injector<
            impl
        >;

        injector().create<c4>();
    }

    return 0;
}

//]

