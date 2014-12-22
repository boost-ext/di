//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_REQUIRES_HPP
#define BOOST_DI_CORE_REQUIRES_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace boost { namespace di { namespace core {

template<class>
class requires_unique_bindings;

template<class... Ts>
class requires_unique_bindings<aux::type_list<Ts...>> {
    template<class T>
    struct expected {
        using type = aux::pair<
            aux::pair<typename T::expected, typename T::name>
          , std::integral_constant<bool, T::scope::priority>
        >;
    };

    pool<aux::type_list<typename expected<Ts>::type...>> bindings;
};

template<class TExpected, class TGiven, class TScope>
class requires_external_concepts {
    static_assert(std::is_same<TExpected, TGiven>{}, "");
    static_assert(std::is_same<TScope, scopes::deduce>{}, "");
};

}}} // boost::di::core

#endif

