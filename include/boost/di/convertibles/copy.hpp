//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_COPY_HPP
#define BOOST_DI_CONVERTIBLES_COPY_HPP

#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_polymorphic.hpp>

#include "boost/di/memory.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

template<typename T, typename = void>
class copy_impl
{
    typedef function<T*()> object_t;

public:
    explicit copy_impl(const object_t& object)
        : object_(object)
    { }

protected:
    object_t object_;
};

template<typename T>
class copy_impl<T, typename disable_if<is_polymorphic<T> >::type>
{
    typedef function<T*()> object_t;

public:
    explicit copy_impl(const object_t& object)
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
class copy : public copy_impl<T>
{
    using copy_impl<T>::object_;

public:
    template<typename U>
    explicit copy(const U& object)
        : copy_impl<T>(object)
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
    operator auto_ptr<I>&() const {
        static auto_ptr<I> s_object_; // not thread safe
        s_object_.reset(object_());
        return s_object_;
    }

    template<typename I, typename TName>
    operator named<auto_ptr<I>, TName>&() const {
        static named<auto_ptr<I>, TName> s_object_;
        s_object_.reset(object_());
        return s_object_;
    }

    template<typename I>
    operator unique_ptr<I>() const {
        return unique_ptr<I>(object_());
    }

    template<typename I, typename TName>
    operator named<unique_ptr<I>, TName>() const {
        return named<unique_ptr<I>, TName>(object_());
    }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

