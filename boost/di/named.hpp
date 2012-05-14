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
#include <boost/utility/enable_if.hpp>
#include "boost/di/aux/make_plain.hpp"
#include "boost/di/aux/has_traits.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

template<
    typename T
  , typename TName = void
  , typename = void
>
class named
{
public:
    typedef typename aux_::make_plain<T>::type value_type;
    typedef named<value_type, TName> element_type;

    named(T value = T()) // non explicit
        : value_(value)
    { }

    named(const shared_ptr<T>& value) // non explicit
        : value_(*value)
    { }

    operator T() const { return value_; }

private:
    T value_;
};

template<
    typename T
  , typename TName
>
class named<T, TName, typename enable_if<is_polymorphic<T> >::type>
{
public:
    typedef typename aux_::make_plain<T>::type value_type;
    typedef named<value_type, TName> element_type;
};

template<
    typename T
  , typename TName
>
class named<T, TName, typename enable_if<aux_::has_element_type<T> >::type>
{
public:
    typedef named<typename aux_::make_plain<T>::type, TName> element_type;
    typedef typename aux_::make_plain<T>::type value_type;

    named(T value = T(new typename T::element_type)) // non explicit
        : value_(value)
    { }

    //TODO
    //named(value_type value) // non explicit
       //: value_(make_shared<value_type>(value))
    //{ }

    operator T() const { return value_; }
/*    operator value_type() const { return *value_; }*/
    //T* operator->() const { return value_.get(); }
    //T& operator*() const { return *value_; }
    /*T* get() const { return value_.get(); }*/

private:
    T value_;
};

} // namespace boost
} // namespace di

#endif

