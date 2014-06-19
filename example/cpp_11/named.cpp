//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[named_cpp_11
//`[h6 C++ 11]
//<-
#include <cassert>
#include <boost/mpl/string.hpp>
//->
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

class named
{
public:
    named(di::named<int, mpl::string<'1'>> i1
        , di::named<int, mpl::string<'2'>> i2)
        : i1(i1), i2(i2) {
        assert(i1 == 42);
        assert(i2 == 87);
    }

private:
    int i1 = 0;
    int i2 = 0;
};

} // namespace

int main() {
    di::injector<
        di::bind_int<42>::named<mpl::string<'1'>>
      , di::bind_int<87>::named<mpl::string<'2'>>
    >().create<named>();

    return 0;
}

//`full code example: [@example/cpp_11/named.cpp named.cpp]
//]

