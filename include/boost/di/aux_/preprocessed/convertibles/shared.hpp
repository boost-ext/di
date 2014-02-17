//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_SHARED_HPP
#define BOOST_DI_CONVERTIBLES_SHARED_HPP

#include "boost/di/aux_/preprocessed/convertibles/shared.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/preprocessed/named.hpp"
#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class shared
{
public:
    shared() { }

    shared(const aux::shared_ptr<T>& object)
        : object_(object)
    { }

    bool operator!() const {
        return !object_;
    }

    void reset(T* ptr = 0) {
        return object_.reset(ptr);
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return object_;
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<const aux::shared_ptr<I>&>&) const {
        return object_;
    }

    template<typename I, typename TName>
    aux::shared_ptr<I> operator()(const type<named<aux::shared_ptr<I>, TName> >&) const {
        return object_;
    }

    template<typename I, typename TName>
    aux::shared_ptr<I> operator()(const type<named<const aux::shared_ptr<I>&, TName> >&) const {
        return object_;
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<aux::weak_ptr<I> >&) const {
        return object_;
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<const aux::weak_ptr<I>&>&) const {
        return object_;
    }

    template<typename I, typename TName>
    aux::weak_ptr<I> operator()(const type<named<aux::weak_ptr<I>, TName> >&) const {
        return aux::weak_ptr<I>(object_);
    }

    template<typename I, typename TName>
    aux::weak_ptr<I>
    operator()(const type<named<const aux::weak_ptr<I>&, TName> >&) const {
        return aux::weak_ptr<I>(object_);
    }

    template<typename I>
    operator I() const {
        return (*this)(type<I>());
    }

private:
    aux::shared_ptr<T> object_;
};

}
}
}

#endif

