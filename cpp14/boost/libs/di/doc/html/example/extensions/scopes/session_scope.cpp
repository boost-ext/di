//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[session_scope
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

//<-
struct interface1 { virtual ~interface1() noexcept = default; };
struct implementation1 : interface1 { };
//->

namespace di = boost::di;

template<class TName, class TScope>
class session_scope {
    class session_guard {
    public:
        explicit session_guard(bool& guard)
            : guard_(guard)
        {
            guard = true;
        }

        ~session_guard() {
            guard_ = false;
        }

    private:
        bool& guard_;
    };

public:
    template<class TExpected, class TGiven>
    class scope {
        using scope_type = typename TScope::template scope<TExpected, TGiven>;

    public:
        template<class T>
        using is_referable = typename scope_type::template is_referable<T>;

        template<class T, class TProvider>
        static auto try_create(const TProvider& provider)
            -> decltype(scope_type{}.template try_create<T>(provider));

        template<class T, class TProvider>
        auto create(const TProvider& provider) {
            static std::shared_ptr<TGiven> null{nullptr};
            return get_session() ? scope_.template create<T>(provider) : null;
        }

    private:
        scope_type scope_;
    };

    auto operator()() const {
        return session_guard{get_session()};
    }

private:
    static auto& get_session() {
        static auto is_in_session = false;
        return is_in_session;
    }
};

template<class TName, class TScope = di::scopes::singleton>
auto session(const TName&, const TScope& = {}) {
    return session_scope<TName, TScope>{};
}

auto my_session = []{};

int main() {
    auto injector = di::make_injector(
        di::bind<interface1>().to<implementation1>().in(session(my_session))
    );

    assert(!injector.create<std::shared_ptr<interface1>>());

    {
    auto ms = session(my_session)();
    assert(injector.create<std::shared_ptr<interface1>>());
    }

    assert(!injector.create<std::shared_ptr<interface1>>());
}

//]

