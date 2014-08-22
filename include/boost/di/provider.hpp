//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDER_HPP
#define BOOST_DI_PROVIDER_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"

#include <utility>
#include <type_traits>

namespace boost {

namespace mpl {
    struct string_tag;
    template<typename> struct c_str;
} // namespace mpl

namespace di {

BOOST_DI_HAS_MEMBER_TYPE(tag);
BOOST_DI_HAS_MEMBER(value);

template<typename, typename = void>
struct is_mpl_string
    : std::false_type
{ };

template<typename T>
struct is_mpl_string<T, typename std::enable_if<has_tag<T>::value>::type>
    : std::is_same<mpl::string_tag, typename T::tag>
{ };

template<typename T>
using is_explicit = bool_<has_value<T>::value || is_mpl_string<T>::value>;

class provider {
public:
    template<typename TExpected, typename TGiven>
    typename std::enable_if<!is_explicit<TGiven>::value, TExpected*>::type
    get() const {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename std::enable_if<has_value<TGiven>::value, TExpected*>::type
    get() const {
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven>
    typename std::enable_if<is_mpl_string<TGiven>::value, TExpected*>::type
    get() const {
        return new TExpected(mpl::c_str<TGiven>::value);
    }

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* get(TArgs&&... args) const {
        return new TGiven(std::forward<TArgs>(args)...);
    }
};

} // namespace di
} // namespace boost

#endif
