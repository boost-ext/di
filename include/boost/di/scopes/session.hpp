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

template<typename /*TName*/>
class session_entry { };

template<typename /*TName*/>
class session_exit { };

template<typename /*TName*/>
class session {
public:
    static constexpr auto priority = 0; // 0 - lowest, N - highest

    template<typename T>
    class scope {
        using result_type = wrappers::shared<T>;

    public:
        template<typename TName>
        void call(const session_entry<TName>&) {
            in_scope_ = true;
        }

        template<typename TName>
        void call(const session_exit<TName>&) {
            in_scope_ = false;
            object_.reset();
        }

        result_type create(T* ptr) {
            if (in_scope_ && !object_) {
                object_.reset(ptr);
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

