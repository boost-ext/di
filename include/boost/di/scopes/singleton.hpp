//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SINGLETON_HPP
#define BOOST_DI_SCOPES_SINGLETON_HPP

#include "boost/di/wrappers/shared.hpp"

namespace boost {
namespace di {
namespace scopes {

class singleton {
public:
    static constexpr auto priority = 0; // 0 - lowest, N - highest

    template<typename>
    struct rebind {
        using other = singleton;
    };

    template<typename T>
    class scope {
        using result_type = wrappers::shared<T>;

    public:
        void create3(int);

        result_type create(T* ptr) {
            if (!get_instance()) {
                get_instance().reset(ptr);
            }
            return get_instance();
        }

    private:
        static result_type& get_instance() {
            static result_type object;
            return object;
        }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

