//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_NAMED_HPP
#define QDEPS_UTILITY_NAMED_HPP

#include <boost/type_traits/is_base_of.hpp>
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
    Named(T p_value = T()) // non explicit
        : m_value(p_value)
    { }

    operator T() { return m_value; }
    T* operator->() const { return m_value; }
    T& operator*() const { return m_value; }
    T* get() const { return m_value; }

private:
    T m_value;
};

} // namespace Utility
} // namespace QDeps

namespace boost
{

template<typename TDerived, typename TBase, typename TName>
struct is_base_of< QDeps::Utility::Named<TBase, TName>, TDerived> : is_base_of<TBase, TDerived>
{ };

template<typename TBase, typename TDerived, typename TName>
struct is_base_of< TBase, QDeps::Utility::Named<TDerived, TName> > : is_base_of<TBase, TDerived>
{ };

} // namespace boost

#endif

