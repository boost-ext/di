//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ALLOCATOR_HPP
#define BOOST_DI_CORE_ALLOCATOR_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/has_value.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)

template<typename, typename = void>
struct is_mpl_string
    : std::false_type
{ };

//template<typename T>
//struct is_mpl_string<T, typename enable_if<has_tag<T>>::type>
    //: is_same<aux::mpl::string_tag, typename T::tag>
//{ };

template<typename T>
using is_explicit = bool_<type_traits::has_value<T>::value || is_mpl_string<T>::value>;

class allocator {
public:
    template<typename TExpected, typename TGiven>
    typename std::enable_if<!is_explicit<TGiven>::value, TExpected*>::type
    allocate() const {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename std::enable_if<type_traits::has_value<TGiven>::value, TExpected*>::type
    allocate() const {
        return new TExpected(TGiven::value);
    }

    //template<typename TExpected, typename TGiven>
    //typename std::enable_if<is_mpl_string<TGiven>::value, TExpected*>::type
    //allocate() const {
        //return new TExpected(aux::mpl::c_str<TGiven>::value);
    //}

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* allocate(TArgs&&... args) const {
        return new TGiven(std::forward<TArgs>(args)...);
    }
};

} // namespace core
} // namespace di
} // namespace boost

#endif
