//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#include "boost/di/aux_/memory.hpp"

#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class shared
{
    template<typename U, typename TShared = aux::shared_ptr<U> >
    class sp_holder
    {
    public:
        explicit sp_holder(const TShared& object)
            : object_(object)
        { }

    private:
        TShared object_;
    };

public:
    shared() { }

    shared(const aux::shared_ptr<T>& object) // non explicit
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
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I> >&) const {
        aux_::shared_ptr<sp_holder<T> > sp(new sp_holder<T>(object_));
        return aux_::shared_ptr<T>(sp, object_.get());
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<aux::weak_ptr<I> >&) const {
        return object_;
    }

private:
    aux::shared_ptr<T> object_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

