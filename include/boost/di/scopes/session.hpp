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

template<typename>
class session_entry { };

template<typename>
class session_exit { };

template<typename T>
class session {
public:
    static const bool priority = false;

    template<typename TExpected>
    class scope {
    public:
        using result_type = wrappers::shared<TExpected>;

        void call(const session_entry<T>&) {
            in_scope_ = true;
        }

        void call(const session_exit<T>&) {
            in_scope_ = false;
            object_.reset();
        }

        result_type create(const std::function<TExpected*()>& f) {
            if (in_scope_ && !object_) {
                object_.reset(f());
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

