//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_SHARED_HPP
#define BOOST_DI_CONVERTIBLES_SHARED_HPP

#include "boost/di/memory.hpp"
#include "boost/di/named.hpp"
#include "boost/config.hpp"

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class shared
{
public:
    shared() { }

    explicit shared(shared_ptr<T> object)
        : object_(object)
    { }

    bool operator!() const {
        return !object_;
    }

    void reset() {
        object_.reset();
    }

    void reset(T* ptr) {
        object_.reset(ptr);
    }

    template<typename I>
    operator shared_ptr<I>() const {
        return object_;
    }

    template<typename I, typename TName>
    operator named<shared_ptr<I>, TName>() const {
        return object_;
    }

    template<typename I>
    operator weak_ptr<I>() const {
        return object_;
    }

    template<typename I, typename TName>
    operator named<weak_ptr<I>, TName>() const {
        return named<weak_ptr<I> >(object_);
    }

private:
    shared_ptr<T> object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

