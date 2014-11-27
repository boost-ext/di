//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SESSION_HPP
#define BOOST_DI_SCOPES_SESSION_HPP

#include "boost/di/wrappers/shared.hpp"

namespace boost {
namespace di {
namespace scopes {

template<class>
class session_entry { };

template<class>
class session_exit { };

template<class>
class session {
public:
    static constexpr auto priority = 0; // 0 - lowest, N - highest

    template<class T>
    class scope {
        using result_type = wrappers::shared<T>;

    public:
        template<class TName>
        void call(const session_entry<TName>&) {
            in_scope_ = true;
        }

        template<class TName>
        void call(const session_exit<TName>&) {
            in_scope_ = false;
            object_.reset();
        }

        template<class, class TProvider>
        result_type create(const TProvider& provider) {
            if (in_scope_ && !object_) {
                object_.reset(provider.get());
            }
            return object_;
        }

    private:
        result_type object_;
        bool in_scope_ = false;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

