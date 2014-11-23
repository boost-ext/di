//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SHARED_HPP
#define BOOST_DI_SCOPES_SHARED_HPP

#include "boost/di/wrappers/shared.hpp"

namespace boost {
namespace di {
namespace scopes {

class shared {
public:
    static constexpr auto priority = 0; // 0 - lowest, N - highest

    template<typename>
    struct rebind {
        using other = shared;
    };

    template<typename T>
    class scope {
        using result_type = wrappers::shared<T>;

    public:
        void create3(int);

        result_type create(T* ptr) {
            if (!object_) {
                object_.reset(ptr);
            }
            return object_;
        }

    private:
        result_type object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

