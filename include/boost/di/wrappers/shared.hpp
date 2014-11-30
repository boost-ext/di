//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#include "boost/di/aux_/memory.hpp"

namespace boost {
namespace di {
namespace wrappers {

template<class T>
class shared {
    template<class U, class TShared = aux::shared_ptr<U>>
    class sp_holder {
    public:
        explicit sp_holder(const TShared& value)
            : value_(value)
        { }

    private:
        TShared value_;
    };

public:
    shared() { }

    shared(const aux::shared_ptr<T>& value) noexcept // non explicit
        : value_(value)
    { }

    inline bool operator!() const noexcept {
        return !value_;
    }

    inline void reset(T* ptr = 0) noexcept {
        return value_.reset(ptr);
    }

    template<class I>
    inline aux::shared_ptr<I> operator()(const aux::type<aux::shared_ptr<I>>&) const noexcept {
        return value_;
    }

    template<class I>
    inline aux_::shared_ptr<I> operator()(const aux::type<aux_::shared_ptr<I>>&) const noexcept {
        aux_::shared_ptr<sp_holder<T>> sp(new sp_holder<T>(value_));
        return aux_::shared_ptr<T>(sp, value_.get());
    }

    template<class I>
    inline aux::weak_ptr<I> operator()(const aux::type<aux::weak_ptr<I>>&) const noexcept {
        return value_;
    }

private:
    aux::shared_ptr<T> value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

