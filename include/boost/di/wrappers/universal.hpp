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
#include "boost/di/named.hpp"
#include "boost/di/type_traits/is_convertible_to_ref.hpp"

#include <vector>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_copy_constructible.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/type.hpp>

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

template<typename TResult, typename T, typename TValueType>
inline typename disable_if<is_copy_constructible<T>, const TResult&>::type
copy(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value) {
    aux::shared_ptr<TResult> object(value(boost::type<T*>()));
    refs.push_back(object);
    return *object;
}

template<typename T>
struct holder
{
    explicit holder(const T& value)
        : held(value)
    { }

    T held;
};

template<typename TResult, typename T, typename TValueType>
inline typename enable_if<is_copy_constructible<T>, const TResult&>::type
copy(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value) {
    aux::shared_ptr<holder<TResult> > object(new holder<TResult>(value(boost::type<T>())));
    refs.push_back(object);
    return object->held;
}

template<typename T>
class universal_impl
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&, const TValueType& value)
        : value_(boost::bind<T>(value, boost::type<T>()))
    { }

    operator T() const {
        return value_();
    }

private:
    function<T()> value_;
};

template<typename T>
class universal_impl<aux::auto_ptr<T> >
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value)
        : value_(new aux::auto_ptr<T>(value(boost::type<aux::auto_ptr<T> >()).release()))
    {
        refs.push_back(aux::shared_ptr<aux::auto_ptr<T> >(value_));
    }

    operator aux::auto_ptr<T>&() {
        return *value_;
    }

private:
    aux::auto_ptr<T>* value_; // weak
};

template<typename T>
class universal_impl<const T&>
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&
                 , const TValueType& value
                 , typename enable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind<const T&>(value, boost::type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename disable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind(&copy<T, T, TValueType>, boost::ref(refs), value))
    { }

    operator const T&() const {
        return value_();
    }

private:
    function<const T&()> value_;
};

template<typename T, typename TName>
class universal_impl<named<T, TName> >
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&, const TValueType& value)
        : value_(boost::bind<T>(value, boost::type<T>()))
    { }

    operator T() const {
        return value_();
    }

    operator named<T, TName>() const {
        return value_();
    }

private:
    function<T()> value_;
};

template<typename T, typename TName>
class universal_impl<named<const T&, TName> >
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&
                 , const TValueType& value
                 , typename enable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind<const T&>(value, boost::type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename disable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind(&copy<T, T, TValueType>, boost::ref(refs), value))
    { }

    operator named<const T&, TName>() const {
        return value_();
    }

private:
    function<named<const T&, TName>()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<T, TName>&>
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value)
        : value_(boost::bind(&copy<named<T, TName>, T, TValueType>, boost::ref(refs), value))
    { }

    operator const named<T, TName>&() const {
        return value_();
    }

private:
    function<const named<T, TName>&()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<const T&, TName>&>
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&
                 , const TValueType& value
                 , typename enable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind<const named<const T&, TName>&>(value, boost::type<const named<const T&, TName>&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename disable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind(&copy<named<const T&, TName>, T, TValueType>, boost::ref(refs), value))
    { }

    operator const named<const T&, TName>&() const {
        return value_();
    }

private:
    function<const named<const T&, TName>&()> value_;
};

} // namespace detail

template<typename T>
class universal : public detail::universal_impl<T>
{
public:
    typedef universal type;
    typedef T element_type;

    template<typename TValueType>
    universal(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value)
        : detail::universal_impl<T>(refs, value)
    { }
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

