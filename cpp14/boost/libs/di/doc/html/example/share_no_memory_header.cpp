//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[share_no_memory_header
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() noexcept = default; };
struct implementation : interface { };
//->

/*<no `include <memory>`>*/
class example {
public:
    example(interface& i1, interface& i2, int i) {
        assert(dynamic_cast<implementation*>(&i1) == dynamic_cast<implementation*>(&i2));
        assert(42 == i);
    }
};

int main() {
    auto injector = di::make_injector(
        di::bind<interface, implementation>()
      , di::bind<int>().to(42)
    );

    injector.create<example>();
}

//]

