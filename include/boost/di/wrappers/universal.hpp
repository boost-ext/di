//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_WRAPPERS_UNIVERSAL_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

BOOST_DI_HAS_METHOD_SIGN(call_operator, opeartor());

template<class TValueType, class T>
using is_convertible_to_ref =
    has_call_operator<TValueType, const T&(TValueType::*)(const aux::type<const T&>&) const>;

template<class TResult, class T, class TWrapper>
inline std::enable_if_t<!std::is_copy_constructible<T>{}, const TResult&>
copy(aux::shared_ptr<void>& refs, const TWrapper& wrapper) noexcept {
    refs.reset(wrapper(aux::type<T*>()));
    return *refs;
}

template<class T>
struct holder {
    explicit holder(const T& wrapper) noexcept
        : held(wrapper)
    { }

    T held;
};

template<class TResult, class T, class TWrapper>
inline std::enable_if_t<std::is_copy_constructible<T>{}, const TResult&>
copy(aux::shared_ptr<void>& refs, const TWrapper& wrapper) noexcept {
    aux::shared_ptr<holder<TResult>> sp(new holder<TResult>(wrapper(aux::type<T>())));
    refs = sp;
    return sp->held;
}

template<class T>
class universal_impl {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&, const TWrapper& wrapper) noexcept
        : object_(wrapper(aux::type<T>()))
    { }

    inline operator T() const noexcept {
        return object_;
    }

    inline operator T&&() noexcept {
        return std::move(object_);
    }

private:
    T object_;
};

template<class T>
class universal_impl<const T&> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&
                 , const TWrapper& wrapper
                 , std::enable_if_t<is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(wrapper(aux::type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<!is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    inline operator const T&() const noexcept {
        return object_;
    }

private:
    std::reference_wrapper<const T> object_;
};

template<class T, class TName>
class universal_impl<named<T, TName>> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&, const TWrapper& wrapper) noexcept
        : object_(wrapper(aux::type<T>()))
    { }

    inline operator T() const noexcept {
        return object_;
    }

    inline operator named<T, TName>() const noexcept {
        return object_;
    }

private:
    T object_;
};

template<class T, class TName>
class universal_impl<named<const T&, TName>> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&
                 , const TWrapper& wrapper
                 , std::enable_if_t<is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(wrapper(aux::type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<!is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    inline operator named<const T&, TName>() const noexcept {
        return object_;
    }

private:
    named<const T&, TName> object_;
};

template<class T, class TName>
class universal_impl<const named<T, TName>&> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper) noexcept
        : object_(copy<named<T, TName>, T, TWrapper>(refs, wrapper))
    { }

    inline operator const named<T, TName>&() const noexcept {
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
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : refs_(refs)
        , object_(wrapper(aux::type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<!is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : refs_(refs)
        , object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    inline operator const named<const T&, TName>&() const noexcept {
        aux::shared_ptr<holder<named<const T&, TName>>> object(
            new holder<named<const T&, TName>>(object_)
        );
        refs_ = object;
        return object->held;
    }

private:
    aux::shared_ptr<void>& refs_;
    named<const T&, TName> object_;
};

} // namespace detail

template<class T>
using universal = detail::universal_impl<T>;

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

