//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_SHARED_HPP
#define BOOST_DI_CONVERTIBLES_SHARED_HPP

#include "boost/di/config.hpp"
#include "boost/di/named.hpp"

#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class shared : public shared_ptr<T>
{
public:
    shared() { }

    explicit shared(const shared_ptr<T>& object)
        : shared_ptr<T>(object)
    { }

    template<typename I>
    shared_ptr<I> operator()(const type<shared_ptr<I> >&) const {
        return *this;
    }

    template<typename I>
    shared_ptr<I> operator()(const type<const shared_ptr<I>&>&) const {
        return *this;
    }

    template<typename I, typename TName>
    named<shared_ptr<I>, TName>
    operator()(const type<const named<shared_ptr<I>, TName>&>&) const {
        return *this;
    }

    template<typename I, typename TName>
    named<shared_ptr<I>, TName>
    operator()(const type<const named<const shared_ptr<I>&, TName>&>&) const {
        return *this;
    }

    template<typename I>
    weak_ptr<I> operator()(const type<weak_ptr<I> >&) const {
        return *this;
    }

    template<typename I>
    weak_ptr<I> operator()(const type<const weak_ptr<I>&>&) const {
        return *this;
    }

    template<typename I, typename TName>
    named<weak_ptr<I>, TName>
    operator()(const type<const named<weak_ptr<I>, TName>&>&) const {
        return named<weak_ptr<I> >(*this);
    }

    template<typename I, typename TName>
    named<weak_ptr<I>, TName>
    operator()(const type<const named<const weak_ptr<I>&, TName>&>&) const {
        return named<weak_ptr<I> >(*this);
    }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

