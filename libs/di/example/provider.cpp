//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <boost/mpl/int.hpp>

namespace di = boost::di;

namespace {

struct transaction
{
    transaction(int i)
        : i(i)
    { }

    int i;
};

} // namespace

int main()
{
}

