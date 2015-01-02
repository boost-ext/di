//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/wrappers/unique.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace scopes {

BOOST_DI_HAS_TYPE(element_type);

class unique {
    template<class T>
    using memory = std::conditional_t<
        std::is_pointer<T>{} ||
        std::is_polymorphic<T>{} ||
        has_element_type<aux::remove_accessors_t<T>>{}
      , type_traits::heap
      , type_traits::stack
    >;

public:
    static constexpr auto priority = false;

    template<class, class>
    class scope {
    public:
        template<class T, class TProvider>
        auto create(const TProvider& provider) const {
            using memory = memory<T>;
            using wrapper = wrappers::unique<decltype(provider.get(memory{}))>;
            return wrapper{provider.get(memory{})};
        }
    };
};

}}} // boost::di::scopes

#endif

