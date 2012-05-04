//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

#if 0
class session_scope
{
public:
    template<typename T>
    class scope
    {
    public:
        template<typename... Args>
        boost::shared_ptr<T> create(Args.. args) {
            return boost::make_shared(args)
        }
    };
};
#endif

int main()
{
}

