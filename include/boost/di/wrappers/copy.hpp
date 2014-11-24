//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_COPY_HPP
#define BOOST_DI_WRAPPERS_COPY_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class copy {
public:
    copy(const T& value) // non explicit
        : value_(value)
    { }

    template<typename I>
    I operator()(const type<I>&, aux::enable_if_t<!std::is_polymorphic<I>::value>* = 0) const {
        return value_;
    }

    template<typename I>
    I* operator()(const type<I*>&) const {
        return new I(value_); // ownership transfer
    }

    template<typename I>
    const I* operator()(const type<const I*>&) const {
        return I(value_); // ownership transfer
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I>>&) const {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I>>&) const {
        return aux::unique_ptr<I>(new I(value_));
    }

private:
    T value_;
};

template<typename T>
class copy<T*> {
public:
    copy(T* value) // non explicit
        : value_(value)
    { }

    template<typename I>
    I operator()(const type<I>&, aux::enable_if_t<!std::is_polymorphic<I>::value>* = 0) const {
		std::unique_ptr<I> ptr(value_);
		return *ptr;
    }

    template<typename I>
    I* operator()(const type<I*>&) const {
        return value_; // ownership transfer
    }

    template<typename I>
    const I* operator()(const type<const I*>&) const {
        return value_; // ownership transfer
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
        return aux::shared_ptr<I>(value_);
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I>>&) const {
        return aux_::shared_ptr<I>(value_);
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I>>&) const {
        return aux::unique_ptr<I>(value_);
    }

private:
	T* value_ = nullptr;
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

