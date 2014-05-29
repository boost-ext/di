//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
struct scope_traits
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T&>
{
    typedef scopes::external<> type;
};

template<typename T>
struct scope_traits<const T&>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T*>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<const T*>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<aux::auto_ptr<T> >
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<aux::shared_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<const aux::shared_ptr<T>&>
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux_::shared_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<const aux_::shared_ptr<T>&>
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux::weak_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<const aux::weak_ptr<T>&>
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux::unique_ptr<T> >
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<const aux::unique_ptr<T>&>
{
    typedef scopes::unique<> type;
};

BOOST_DI_FEATURE(RVALUE_REFERENCES)(
    template<typename T>
    struct scope_traits<T&&>
    {
        typedef scopes::unique<> type;
    };

    template<typename T>
    struct scope_traits<const T&&>
    {
        typedef scopes::unique<> type;
    };
)

template<typename T, typename TName>
struct scope_traits<named<T, TName> >
{
    typedef typename scope_traits<T>::type type;
};

template<typename T, typename TName>
struct scope_traits<const named<T, TName>&>
{
    typedef typename scope_traits<T>::type type;
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

