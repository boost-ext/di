//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_SHARED_HPP
#define BOOST_DI_CONVERTIBLES_SHARED_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class shared : public aux::shared_ptr<T>
{
public:
    shared() { }

    shared(const aux::shared_ptr<T>& object) // non explicit
        : aux::shared_ptr<T>(object)
    { }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return *this;
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<const aux::shared_ptr<I>&>&) const {
        return *this;
    }

    template<typename I, typename TName>
    aux::shared_ptr<I> operator()(const type<const named<aux::shared_ptr<I>, TName>&>&) const {
        return *this;
    }

    template<typename I, typename TName>
    aux::shared_ptr<I> operator()(const type<const named<const aux::shared_ptr<I>&, TName>&>&) const {
        return *this;
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<aux::weak_ptr<I> >&) const {
        return *this;
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<const aux::weak_ptr<I>&>&) const {
        return *this;
    }

    template<typename I, typename TName>
    aux::weak_ptr<I> operator()(const type<const named<aux::weak_ptr<I>, TName>&>&) const {
        return aux::weak_ptr<I>(*this);
    }

    template<typename I, typename TName>
    aux::weak_ptr<I>
    operator()(const type<const named<const aux::weak_ptr<I>&, TName>&>&) const {
        return aux::weak_ptr<I>(*this);
    }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

