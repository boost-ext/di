//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"

#include <utility>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

template<
    typename T
  , typename TName = void
  , typename = void
>
class named
{
    typedef typename remove_reference<T>::type& ref_type;

    //named& operator=(const named&);

public:
    typedef T named_type;
    typedef TName name;

    named(const typename remove_reference<T>::type& object) // non explicit
        : object_(object)
    { }

    BOOST_DI_FEATURE(RVALUE_REFERENCES)(
        named(typename remove_reference<T>::type&& object) // non explicit
            : object_(std::move(object))
        { }
    )

    operator T() const {
        return object_;
    }

    operator ref_type() {
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
    //named& operator=(const named&);

public:
    typedef T named_type;
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
    typedef typename type_traits::make_plain<T>::type value_type;

    //named& operator=(const named&);

public:
    typedef T named_type;
    typedef TName name;

    named() { }

    named(const typename remove_reference<T>::type& object) // non explicit
        : object_(object)
    { }

    named(typename type_traits::remove_accessors<T>::type::element_type* ptr) // non explicit
        : object_(ptr)
    { }

    BOOST_DI_FEATURE(RVALUE_REFERENCES)(
        named(typename remove_reference<T>::type&& object) // non explicit
            : object_(std::move(object))
        { }

        //TODO
        template<typename I>
        operator aux::unique_ptr<I>() { return std::move(object_); }
    )

    operator T() const { return object_; }

    value_type* operator->() const { return object_.get(); }
    value_type& operator*() const { return *object_; }
    value_type* get() const { return object_.get(); }

    void reset() {
        object_.reset();
    }

    void reset(typename type_traits::remove_accessors<T>::type::element_type* ptr) {
        object_.reset(ptr);
    }

private:
    T object_;
};

} // namespace di
} // namespace boost

#endif

