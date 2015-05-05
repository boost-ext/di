//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/session.hpp"
#include "common/fakes/fake_provider.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

test create = [] {
    session<>::scope<int, int> session1;
    session<>::scope<int, int> session2;

    {
    std::shared_ptr<int> object1 = session1.create<int>(fake_provider<int>{});
    std::shared_ptr<int> object2 = session1.create<int>(fake_provider<int>{});
    expect_eq(object1, object2);
    }

    {
    std::shared_ptr<int> object1 = session2.create<int>(fake_provider<int>{});
    std::shared_ptr<int> object2 = session2.create<int>(fake_provider<int>{});
    expect_eq(object1, object2);
    }

    {
    std::shared_ptr<int> object1 = session1.create<int>(fake_provider<int>{});
    std::shared_ptr<int> object2 = session2.create<int>(fake_provider<int>{});
    expect_eq(object1, object2);
    }
};

test call = [] {
    struct name { };
    session<name>::scope<int, int> s;
    expect_eq(nullptr, static_cast<std::shared_ptr<int>>(s.create<int>(fake_provider<int>{})));
    s.call(session_entry<name>{});
    expect_neq(nullptr, static_cast<std::shared_ptr<int>>(s.create<int>(fake_provider<int>{})));
    s.call(session_exit<name>{});
    expect_eq(nullptr, static_cast<std::shared_ptr<int>>(s.create<int>(fake_provider<int>{})));
    s.call(session_entry<name>{});
    expect_neq(nullptr, static_cast<std::shared_ptr<int>>(s.create<int>(fake_provider<int>{})));
    s.call(session_exit<name>{});
    expect_eq(nullptr, static_cast<std::shared_ptr<int>>(s.create<int>(fake_provider<int>{})));
};

}}}} // boost::di::v1::scopes

