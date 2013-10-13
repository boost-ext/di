//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_COPY_HPP
#define BOOST_DI_CONVERTIBLES_COPY_HPP

#include "boost/di/memory.hpp"
#include "boost/di/named.hpp"

#include <boost/type.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_polymorphic.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class copy
{
    typedef function<T*()> object_t;

public:
    template<typename I>
    explicit copy(const I& object)
        : object_(object)
    { }

    template<typename I>
    I convert(const type<I>&, typename disable_if<is_polymorphic<I> >::type* = 0) const {
        scoped_ptr<I> ptr(object_());
        return *ptr;
    }

    template<typename I>
    const I convert(const type<const I>&, typename disable_if<is_polymorphic<I> >::type* = 0) const {
        scoped_ptr<I> ptr(object_());
        return *ptr;
    }

    template<typename I>
    I* convert(const type<I*>&) const {
        return object_();
    }

    template<typename I, typename TName>
    named<I, TName> convert(const type<const named<I, TName>&>&) const {
        scoped_ptr<I> ptr(object_());
        return *ptr;
    }

    template<typename I>
    shared_ptr<I> convert(const type<shared_ptr<I> >&) const {
        return shared_ptr<I>(object_());
    }

    template<typename I>
    shared_ptr<I> convert(const type<const shared_ptr<I>&>&) const {
        return shared_ptr<I>(object_());
    }

    template<typename I, typename TName>
    I* convert(const type<const named<shared_ptr<I>, TName>&>&) const {
        return object_();
    }

    template<typename I>
    auto_ptr<I> convert(const type<auto_ptr<I> >&) const {
        return auto_ptr<I>(object_());
    }

    template<typename I, typename TName>
    I* convert(const type<const named<auto_ptr<I>, TName>&>&) const {
        return object_();
    }

    template<typename I>
    unique_ptr<I> convert(const type<unique_ptr<I> >&) const {
        return unique_ptr<I>(object_());
    }

    template<typename I, typename TName>
    I* convert(type<const named<unique_ptr<I>, TName>&>&) const {
        return object_();
    }

private:
    object_t object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

