//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[deduce_scope_cpp_11
//````C++11```
//<-
#include <cassert>
#include <memory>
#include <boost/shared_ptr.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() { } virtual void dummy() = 0; };
struct implementation : interface { void dummy() override { } };
//->

struct dependency1 {
    dependency1(std::shared_ptr<interface> spi /*shared*/, const boost::shared_ptr<interface>& spi_ /*shared*/)
        : spi_(spi), spi__(spi_)
    {
        assert(spi.get() == spi_.get());
    }

    std::shared_ptr<interface> spi_;
    boost::shared_ptr<interface> spi__;
};

struct dependency2 {
    dependency2(std::shared_ptr<interface> spi/*shared*/, int i/*unique*/)
        : spi_(spi)
    {
        assert(i == 0);
    }

    std::shared_ptr<interface> spi_;
};

struct example {
    example(std::unique_ptr<dependency1> dependency1_ /*unique*/
     , const dependency2& dependency2_ /*unique temporary*/)
    {
        assert(dependency2_.spi_.get() == dependency1_->spi_.get());
        assert(dependency2_.spi_.get() == dependency1_->spi__.get());
    }
};

int main() {
    {
        /*<<create injector with deduced `interface`>>*/
        auto injector = di::make_injector(
            di::deduce<implementation>()
        );

        /*<<create `example`>>*/
        injector.create<example>();
    }

    {
        /*<<create injector with deduced `interface`>>*/
        using injector = di::injector<
            implementation
        >;

        /*<<create `example`>>*/
        injector().create<example>();
    }

    return 0;
}

//]

