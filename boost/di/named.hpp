//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include "boost/di/config.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/has_traits.hpp"

namespace boost {
namespace di {

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

    named(T value = T()) // non explicit
        : value_(value)
    { }

    named(const shared_ptr<T>& value) // non explicit
        : value_(*value)
    { }

    operator T() const {
        return value_;
    }

private:
    T value_;
};

template<
    typename T
  , typename TName
>
class named<
    T
  , TName
  , typename enable_if<
        is_polymorphic<T>
    >::type
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
class named<
    T
  , TName
  , typename enable_if<
        type_traits::has_element_type<T>
    >::type
>
{
public:
    typedef named<typename type_traits::make_plain<T>::type, TName> element_type;
    typedef typename type_traits::make_plain<T>::type value_type;
    typedef TName name;

    named(T value = T(new typename T::element_type)) // non explicit
        : value_(value)
    { }

    operator T() const { return value_; }
    value_type* operator->() const { return value_.get(); }
    value_type& operator*() const { return *value_; }
    value_type* get() const { return value_.get(); }

private:
    T value_;
};

} // namespace di
} // namespace boost

#endif

