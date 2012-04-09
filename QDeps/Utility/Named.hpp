//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_NAMED_HPP
#define QDEPS_UTILITY_NAMED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include "QDeps/Back/Aux/Utility.hpp"
#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Utility
{

/**
 * Example: Named< int, mpl::string<'Port'> >
 */
template<typename T, typename TName>
class Named
{
public:
    typedef typename Back::Aux::MakePlain<T>::type value_type;
    typedef Named<value_type, TName> element_type;

    Named(T p_value = T()) // non explicit
        : m_value(new T(p_value))
    { }

    Named(boost::shared_ptr<T> p_value) // non explicit
        : m_value(p_value)
    { }

    operator T() const { return *m_value; }

private:
    boost::shared_ptr<T> m_value;
};

template<typename T, typename TName>
class Named< boost::shared_ptr<T>, TName>
{
public:
    typedef Named<typename Back::Aux::MakePlain<T>::type, TName> element_type;
    typedef typename Back::Aux::MakePlain<T>::type value_type;

    Named(boost::shared_ptr<T> p_value = boost::make_shared<T>()) // non explicit
        : m_value(p_value)
    { }

    Named(value_type p_value) // non explicit
       : m_value(boost::make_shared<value_type>(p_value))
    { }

    operator boost::shared_ptr<T>() const { return m_value; }
    operator value_type() const { return *m_value; }
    T* operator->() const { return m_value.get(); }
    T& operator*() const { return *m_value; }
    T* get() const { return m_value.get(); }

private:
    boost::shared_ptr<T> m_value;
};

template<typename T, typename TName>
class Named< const boost::shared_ptr<T>&, TName> : Named< boost::shared_ptr<T>, TName>
{
public:
    typedef Named<typename Back::Aux::MakePlain<T>::type, TName> element_type;
    typedef typename Back::Aux::MakePlain<T>::type value_type;

    Named(const boost::shared_ptr<T>& p_value) // non explicit
        : Named< boost::shared_ptr<T>, TName>(p_value)
    { }
};

} // namespace Utility
} // namespace QDeps

//namespace boost
//{

//template<typename TBase, typename TDerived, typename TName>
//struct is_same<TBase, QDeps::Utility::Named<TDerived, TName> > : is_same<TBase, TDerived>
//{ };

//template<typename TBase, typename TDerived, typename TName>
//struct is_same<QDeps::Utility::Named<TBase, TName>, QDeps::Utility::Named<TDerived, TName> > : is_same<TBase, TDerived>
//{ };

//template<typename TBase, typename TDerived, typename TName>
//struct is_base_of<TBase, QDeps::Utility::Named<TDerived, TName> > : is_base_of<TBase, TDerived>
//{ };

//} // namespace boost

#endif

