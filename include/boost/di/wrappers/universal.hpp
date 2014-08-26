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
#include <functional>

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

template<typename TResult, typename T, typename TValueType>
inline typename std::enable_if<!std::is_copy_constructible<T>::value, const TResult&>::type
copy(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value) {
    aux::shared_ptr<TResult> object(value(type<T*>()));
    refs.push_back(object);
    return *object;
}

template<typename T>
struct holder {
    explicit holder(const T& value)
        : held(value)
    { }

    T held;
};

template<typename TResult, typename T, typename TValueType>
inline typename std::enable_if<std::is_copy_constructible<T>::value, const TResult&>::type
copy(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value) {
    aux::shared_ptr<holder<TResult>> object(new holder<TResult>(value(type<T>())));
    refs.push_back(object);
    return object->held;
}

template<typename T>
class universal_impl {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TValueType& value)
        : value_(std::bind<T>(value, type<T>()))
    { }

    operator T() const {
        return value_();
    }

private:
    std::function<T()> value_;
};

template<typename T>
class universal_impl<aux::auto_ptr<T>> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value)
        : value_(new aux::auto_ptr<T>(value(type<aux::auto_ptr<T>>()).release()))
    {
        refs.push_back(aux::shared_ptr<aux::auto_ptr<T>>(value_));
    }

    operator aux::auto_ptr<T>&() {
        return *value_;
    }

private:
    aux::auto_ptr<T>* value_; // weak
};

template<typename T>
class universal_impl<const T&> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TValueType& value
                 , typename std::enable_if<detail::has_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind<const T&>(value, type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<!detail::has_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind(&copy<T, T, TValueType>, std::ref(refs), value))
    { }

    operator const T&() const {
        return value_();
    }

private:
    std::function<const T&()> value_;
};

template<typename T, typename TName>
class universal_impl<named<T, TName>> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TValueType& value)
        : value_(std::bind<T>(value, type<T>()))
    { }

    operator T() const {
        return value_();
    }

    operator named<T, TName>() const {
        return value_();
    }

private:
    std::function<T()> value_;
};

template<typename T, typename TName>
class universal_impl<named<const T&, TName>> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TValueType& value
                 , typename std::enable_if<detail::has_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind<const T&>(value, type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<!detail::has_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind(&copy<T, T, TValueType>, std::ref(refs), value))
    { }

    operator named<const T&, TName>() const {
        return value_();
    }

private:
    std::function<named<const T&, TName>()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<T, TName>&> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value)
        : value_(std::bind(&copy<named<T, TName>, T, TValueType>, std::ref(refs), value))
    { }

    operator const named<T, TName>&() const {
        return value_();
    }

private:
    std::function<const named<T, TName>&()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<const T&, TName>&> {
    universal_impl& operator=(const universal_impl&);

public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<detail::has_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : refs_(refs)
        , value_(std::bind<const T&>(value, type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<!detail::has_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : refs_(refs)
        , value_(std::bind(&copy<T, T, TValueType>, std::ref(refs), value))
    { }

    operator const named<const T&, TName>&() const {
        aux::shared_ptr<holder<named<const T&, TName>>> object(
            new holder<named<const T&, TName>>(value_())
        );
        refs_.push_back(object);
        return object->held;
    }

private:
    std::vector<aux::shared_ptr<void>>& refs_;
    std::function<named<const T&, TName>()> value_;
};

} // namespace detail

template<typename T>
class universal {
public:
    template<typename TValueType>
    universal(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value)
        : value_(value(type<T>()))
     { }

    operator T() const {
        return value_;
    }

    T value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

