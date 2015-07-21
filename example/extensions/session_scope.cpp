
//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SESSION_HPP
#define BOOST_DI_SCOPES_SESSION_HPP

#include <memory>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/fwd.hpp" // no_name

namespace boost { namespace di { inline namespace v1 { namespace scopes {

template<class = no_name>
class session_entry { };

template<class = no_name>
class session_exit { };

template<class TName = no_name>
class session {
public:
    template<class, class T>
    class scope {
    public:
        template<class T_>
        using is_referable = typename wrappers::shared<T>::template is_referable<T_>;

        void call(const session_entry<TName>&) noexcept {
            in_scope_ = true;
        }

        void call(const session_exit<TName>&) noexcept {
            in_scope_ = false;
            object_.reset();
        }

        template<class, class TProvider>
        auto try_create(const TProvider& provider)
            -> decltype(wrappers::shared<T>{std::shared_ptr<T>{provider.get()}});

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            if (BOOST_DI_LIKELY(in_scope_) && BOOST_DI_UNLIKELY(!object_)) {
                object_ = std::shared_ptr<T>{provider.get()};
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
        bool in_scope_ = false;
    };
};

}}}} // boost::di::v1::scopes

#endif

//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

auto name = []{};

test session_call = [] {
    auto injector = di::make_injector(
        di::bind<i1, impl1>().in(di::session(name))
    );

    {
    injector.call(di::session_entry(name));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(object.get());
    expect(dynamic_cast<i1*>(object.get()));
    }

    {
    injector.call(di::session_exit(int{}));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(object.get());
    }

    {
    injector.call(di::session_exit(name));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(!object.get());
    }
};

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

