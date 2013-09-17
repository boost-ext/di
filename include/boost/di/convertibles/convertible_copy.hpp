//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_CONVERTIBLE_COPY_HPP
#define BOOST_DI_CONVERTIBLES_CONVERTIBLE_COPY_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_polymorphic.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

template<
    typename T
  , typename = void
>
class convertible_copy_impl
{
    typedef function<T*()> object_t;

public:
    explicit convertible_copy_impl(const object_t& object)
        : object_(object)
    { }

protected:
    object_t object_;
};

template<typename T>
class convertible_copy_impl<T, typename disable_if<is_polymorphic<T> >::type>
{
    typedef function<T*()> object_t;

public:
    explicit convertible_copy_impl(const object_t& object)
        : object_(object)
    { }

    operator T() const {
        scoped_ptr<T> ptr(object_());
        return *ptr;
    }

protected:
    object_t object_;
};

template<typename T>
class convertible_copy : public convertible_copy_impl<T>
{
    using convertible_copy_impl<T>::object_;

public:
    template<typename I>
    explicit convertible_copy(const I& object)
        : convertible_copy_impl<T>(object)
    { }

    operator T*() const {
        return object_();
    }

    template<typename I, typename TName>
    operator named<I, TName>() const {
        scoped_ptr<T> ptr(object_());
        return *ptr;
    }

    template<typename I>
    operator shared_ptr<I>() const {
        return shared_ptr<I>(object_());
    }

    template<typename I, typename TName>
    operator named<shared_ptr<I>, TName>() const {
        return shared_ptr<I>(object_());
    }

    template<typename I>
    operator std::auto_ptr<I>&() const {
        static std::auto_ptr<I> s_object_; // not thread safe
        s_object_.reset(object_());
        return s_object_;
    }

    template<typename I, typename TName>
    operator named<std::auto_ptr<I>, TName>() const {
        static std::auto_ptr<I> s_object_; // not thread safe
        s_object_.reset(object_());
        return s_object_;
    }

#if !defined(BOOST_NO_CXX11_SMART_PTR)
    template<typename I>
    operator std::shared_ptr<I>() const {
        return std::shared_ptr<I>(object_());
    }

    template<typename I, typename TName>
    operator named<std::shared_ptr<I>, TName>() const {
        return named<std::shared_ptr<I>, TName>(object_());
    }

    template<typename I>
    operator std::unique_ptr<I>() const {
        return std::unique_ptr<I>(object_());
    }

    template<typename I, typename TName>
    operator named<std::unique_ptr<I>, TName>() const {
        return named<std::unique_ptr<I>, TName>(object_());
    }
#endif
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

