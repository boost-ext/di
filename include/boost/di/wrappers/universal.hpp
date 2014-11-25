//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_WRAPPERS_UNIVERSAL_HPP

#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

template<class TResult, class T, class TWrapper>
inline aux::enable_if_t<!std::is_copy_constructible<T>{}, const TResult&>
copy(std::vector<aux::shared_ptr<void>>& refs, const TWrapper& wrapper) {
    aux::shared_ptr<TResult> object(wrapper(type<T*>()));
    refs.push_back(object);
    return *object;
}

template<class T>
struct holder {
    explicit holder(const T& wrapper)
        : held(wrapper)
    { }

    T held;
};

template<class TResult, class T, class TWrapper>
inline aux::enable_if_t<std::is_copy_constructible<T>{}, const TResult&>
copy(std::vector<aux::shared_ptr<void>>& refs, const TWrapper& wrapper) {
    aux::shared_ptr<holder<TResult>> object(new holder<TResult>(wrapper(type<T>())));
    refs.push_back(object);
    return object->held;
}

template<class T>
class universal_impl {
public:
    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TWrapper& wrapper)
        : object_(wrapper(type<T>()))
    { }

    operator T() const {
        return object_;
    }

	operator T&&() {
		return std::move(object_);
	}

private:
    T object_;
};

template<class T>
class universal_impl<const T&> {
public:
    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TWrapper& wrapper
                 , aux::enable_if_t<aux::is_convertible_to_ref<TWrapper, T>{}>* = 0)
        : object_(wrapper(type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , aux::enable_if_t<!aux::is_convertible_to_ref<TWrapper, T>{}>* = 0)
        : object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    operator const T&() const {
        return object_;
    }

private:
    std::reference_wrapper<const T> object_;
};

template<class T, class TName>
class universal_impl<named<T, TName>> {
public:
    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TWrapper& wrapper)
        : object_(wrapper(type<T>()))
    { }

    operator T() const {
        return object_;
    }

    operator named<T, TName>() const {
        return object_;
    }

private:
    T object_;
};

template<class T, class TName>
class universal_impl<named<const T&, TName>> {
public:
    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TWrapper& wrapper
                 , aux::enable_if_t<aux::is_convertible_to_ref<TWrapper, T>{}>* = 0)
        : object_(wrapper(type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , aux::enable_if_t<!aux::is_convertible_to_ref<TWrapper, T>{}>* = 0)
        : object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    operator named<const T&, TName>() const {
        return object_;
    }

private:
    named<const T&, TName> object_;
};

template<class T, class TName>
class universal_impl<const named<T, TName>&> {
public:
    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper)
        : object_(copy<named<T, TName>, T, TWrapper>(refs, wrapper))
    { }

    operator const named<T, TName>&() const {
        return object_;
    }

private:
    std::reference_wrapper<const named<T, TName>> object_;
};

template<class T, class TName>
class universal_impl<const named<const T&, TName>&> {
    universal_impl& operator=(const universal_impl&);

public:
    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , aux::enable_if_t<aux::is_convertible_to_ref<TWrapper, T>{}>* = 0)
        : refs_(refs)
        , object_(wrapper(type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , aux::enable_if_t<!aux::is_convertible_to_ref<TWrapper, T>{}>* = 0)
        : refs_(refs)
        , object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    operator const named<const T&, TName>&() const {
        aux::shared_ptr<holder<named<const T&, TName>>> object(
            new holder<named<const T&, TName>>(object_)
        );
        refs_.push_back(object);
        return object->held;
    }

private:
    std::vector<aux::shared_ptr<void>>& refs_;
    named<const T&, TName> object_;
};

} // namespace detail

template<class T>
using universal = detail::universal_impl<T>;

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

