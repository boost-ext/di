//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#include <new>
#include "boost/di/aux_/memory.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T>
class shared {
    template<class TSharedPtr>
    struct sp_holder {
        TSharedPtr object;

        void operator()(...) noexcept {
            object.reset();
        }
    };

public:
    shared() noexcept { }

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
    inline operator aux::shared_ptr<I>() const noexcept {
        return value_;
    }

    template<class I>
    inline operator aux_::shared_ptr<I>() const noexcept {
        using sp = sp_holder<aux_::shared_ptr<I>>;
        if (auto* deleter = std::get_deleter<sp, I>(value_)) {
            return deleter->object;
        } else {
            return {value_.get(), sp_holder<aux::shared_ptr<T>>{value_}};
        }
    }

    template<class I>
    inline operator aux::weak_ptr<I>() const noexcept {
        return value_;
    }

private:
    aux::shared_ptr<T> value_;
};

}}} // namespace boost::di::wrappers

#endif

