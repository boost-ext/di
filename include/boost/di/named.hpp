//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/utility/enable_if.hpp>

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

template<
    typename T = void
  , typename TName = void
  , typename = void
>
class named
{
public:
    typedef typename type_traits::make_plain<T>::type value_type;
    typedef named<value_type, TName> element_type;
    typedef TName name;

    named(T object = T()) // non explicit
        : object_(object)
    { }

    named(const shared_ptr<T>& object) // non explicit
        : object_(*object)
    { }

    operator T() const {
        return object_;
    }

    operator T&() {
        return object_;
    }

private:
    T object_;
};

template<
    typename T
  , typename TName
>
class named<T, TName, typename enable_if<
    is_polymorphic<typename type_traits::remove_accessors<T>::type> >::type
>
{
public:
    typedef typename type_traits::make_plain<T>::type value_type;
    typedef named<value_type, TName> element_type;
    typedef TName name;
};

template<
    typename T
  , typename TName
>
class named<T, TName, typename enable_if<
    has_element_type<typename type_traits::remove_accessors<T>::type> >::type
 >
{
    typedef typename type_traits::remove_accessors<T>::type object_type;

public:
    typedef typename type_traits::make_plain<T>::type value_type;
    typedef named<typename type_traits::make_plain<T>::type, TName> element_type;
    typedef TName name;

    named(T object = T(new typename T::element_type)) // non explicit
        : object_(object)
    { }

    operator T() const { return object_; }

    value_type* operator->() const { return object_.get(); }
    value_type& operator*() const { return *object_; }
    value_type* get() const { return object_.get(); }

private:
    object_type object_;
};

} // namespace di
} // namespace boost

#endif

