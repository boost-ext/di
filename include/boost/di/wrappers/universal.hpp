//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_DETAIL_WRAPPERS_UNIVERSAL_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/named.hpp"

#include <vector>

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

BOOST_DI_HAS_MEMBER_IMPL(
    convertible_to_ref
  , operator()
  , void dummy() { }
  , const TDst&(T::*)(const type<const TDst&>&) const
  , no_tag
  , typename TDst
);

template<typename TResult, typename T, typename TWrapper>
inline typename std::enable_if<!std::is_copy_constructible<T>::wrapper, const TResult&>::type
copy(std::vector<aux::shared_ptr<void>>& refs, const TWrapper& wrapper) {
    aux::shared_ptr<TResult> object(wrapper(type<T*>()));
    refs.push_back(object);
    return *object;
}

template<typename T>
struct holder {
    explicit holder(const T& wrapper)
        : held(wrapper)
    { }

    T held;
};

template<typename TResult, typename T, typename TWrapper>
inline typename std::enable_if<std::is_copy_constructible<T>::wrapper, const TResult&>::type
copy(std::vector<aux::shared_ptr<void>>& refs, const TWrapper& wrapper) {
    aux::shared_ptr<holder<TResult>> object(new holder<TResult>(wrapper(type<T>())));
    refs.push_back(object);
    return object->held;
}

template<typename T>
class universal_impl {
public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TWrapper& wrapper)
        : wrapper_(wrapper(type<T>()))
    { }

    operator T() const {
        return wrapper_;
    }

private:
    T wrapper_;
};

template<typename T>
class universal_impl<aux::auto_ptr<T>> {
public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs, const TWrapper& wrapper)
        : wrapper_(new aux::auto_ptr<T>(wrapper(type<aux::auto_ptr<T>>()).release()))
    {
        refs.push_back(aux::shared_ptr<aux::auto_ptr<T>>(wrapper_));
    }

    operator aux::auto_ptr<T>&() {
        return *wrapper_;
    }

private:
    aux::auto_ptr<T>* wrapper_; // weak
};

template<typename T>
class universal_impl<const T&> {
public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TWrapper& wrapper
                 , typename std::enable_if<has_convertible_to_ref<TWrapper, T>::wrapper>::type* = 0)
        : wrapper_(wrapper(type<const T&>()))
    { }

    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , typename std::enable_if<!has_convertible_to_ref<TWrapper, T>::wrapper>::type* = 0)
        : wrapper_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    operator const T&() const {
        return wrapper_;
    }

private:
    std::reference_wrapper<const T> wrapper_;
};

template<typename T, typename TName>
class universal_impl<named<T, TName>> {
public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TWrapper& wrapper)
        : wrapper_(wrapper(type<T>()))
    { }

    operator T() const {
        return wrapper_;
    }

    operator named<T, TName>() const {
        return wrapper_;
    }

private:
    T wrapper_;
};

template<typename T, typename TName>
class universal_impl<named<const T&, TName>> {
public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TWrapper& wrapper
                 , typename std::enable_if<has_convertible_to_ref<TWrapper, T>::wrapper>::type* = 0)
        : wrapper_(wrapper(type<const T&>()))
    { }

    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , typename std::enable_if<!has_convertible_to_ref<TWrapper, T>::wrapper>::type* = 0)
        : wrapper_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    operator named<const T&, TName>() const {
        return wrapper_;
    }

private:
    named<const T&, TName> wrapper_;
};

template<typename T, typename TName>
class universal_impl<const named<T, TName>&> {
public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper)
        : wrapper_(copy<named<T, TName>, T, TWrapper>(refs, wrapper))
    { }

    operator const named<T, TName>&() const {
        return wrapper_;
    }

private:
    std::reference_wrapper<const named<T, TName>> wrapper_;
};

template<typename T, typename TName>
class universal_impl<const named<const T&, TName>&> {
    universal_impl& operator=(const universal_impl&);

public:
    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , typename std::enable_if<has_convertible_to_ref<TWrapper, T>::wrapper>::type* = 0)
        : refs_(refs)
        , wrapper_(wrapper(type<const T&>()))
    { }

    template<typename TWrapper>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TWrapper& wrapper
                 , typename std::enable_if<!has_convertible_to_ref<TWrapper, T>::wrapper>::type* = 0)
        : refs_(refs)
        , wrapper_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    operator const named<const T&, TName>&() const {
        aux::shared_ptr<holder<named<const T&, TName>>> object(
            new holder<named<const T&, TName>>(wrapper_)
        );
        refs_.push_back(object);
        return object->held;
    }

private:
    std::vector<aux::shared_ptr<void>>& refs_;
    named<const T&, TName> wrapper_;
};

} // namespace detail

template<typename T>
using universal = detail::universal_impl<T>;

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

