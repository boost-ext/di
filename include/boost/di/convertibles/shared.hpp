//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_SHARED_HPP
#define BOOST_DI_CONVERTIBLES_SHARED_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

#include "boost/di/named.hpp"
#include "boost/config.hpp"

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class shared
{
    template<typename U, typename TShared = shared_ptr<U> >
    class sp_holder
    {
    public:
        explicit sp_holder(TShared object)
            : object_(object)
        { }

    private:
        TShared object_;
    };

public:
    shared() { }

    explicit shared(shared_ptr<T> object)
        : object_(object)
    { }

#if !defined(BOOST_NO_CXX11_SMART_PTR)
    explicit shared(std::shared_ptr<T> object)
    {
        shared_ptr<sp_holder<T, std::shared_ptr<T> > > sp =
            make_shared<sp_holder<T, std::shared_ptr<T> > >(object);
        object_ = shared_ptr<T>(sp, object.get());
    }
#endif

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

#if !defined(BOOST_NO_CXX11_SMART_PTR)
    template<typename I>
    operator std::shared_ptr<I>() const {
        std::shared_ptr<sp_holder<T> > sp = std::make_shared<sp_holder<T> >(object_);
        return std::shared_ptr<T>(sp, object_.get());
    }

    template<typename I, typename TName>
    operator named<std::shared_ptr<I>, TName>() const {
        std::shared_ptr<sp_holder<T> > sp = std::make_shared<sp_holder<T> >(object_);
        return named<std::shared_ptr<I>, TName>(
            std::shared_ptr<T>(sp, object_.get())
        );
    }

    template<typename I>
    operator std::weak_ptr<I>() const {
        std::shared_ptr<sp_holder<T> > sp = std::make_shared<sp_holder<T> >(object_);
        return std::weak_ptr<I>(std::shared_ptr<T>(sp, object_.get()));
    }

    template<typename I, typename TName>
    operator named<std::weak_ptr<I>, TName>() const {
        std::shared_ptr<sp_holder<T> > sp = std::make_shared<sp_holder<T> >(object_);
        return named<std::weak_ptr<I>, TName>(
            std::weak_ptr<I>(std::shared_ptr<T>(sp, object_.get()))
        );
    }
#endif

private:
    shared_ptr<T> object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

