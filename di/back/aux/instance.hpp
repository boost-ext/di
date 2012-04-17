//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_AUX_INSTANCE_HPP
#define DI_BACK_AUX_INSTANCE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace di
{
namespace back
{
namespace aux
{

namespace detail
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)

template<typename T, typename = void>
struct GetvalueType
{
    typedef T type;
};

template<>
struct GetvalueType<std::string, void>
{
    typedef std::string type;
};

template<typename T>
struct GetvalueType<T, typename boost::enable_if<has_value_type<T> >::type>
{
    typedef typename T::value_type type;
};

} // namespace detail

template
<
    typename T,
    typename TContext = boost::mpl::vector0<>,
    typename Enable = void
>
class Instance
{
public:
    typedef T valueType;
    typedef boost::variant<const T&, T&, boost::shared_ptr<T> > result_type;

    explicit Instance(const T& p_member)
        : m_member(p_member)
    { }

    explicit Instance(T& p_member)
        : m_member(p_member)
    { }

    explicit Instance(boost::shared_ptr<T> p_member)
        : m_member(p_member)
    { }

    result_type get() const
    {
        return m_member;
    }

private:
    result_type m_member;
};

template
<
    typename T,
    typename TContext
>
class Instance
    <
        T, TContext,
        typename boost::enable_if
        <
            boost::mpl::or_
            <
                boost::is_same<typename detail::GetvalueType<T>::type, std::string>,
                boost::is_pod<typename detail::GetvalueType<T>::type>
            >
        >::type
    >
{
public:
    typedef T valueType;
    typedef typename detail::GetvalueType<T>::type result_type;

    explicit Instance(result_type p_member)
        : m_member(p_member)
    { }

    result_type get() const
    {
        return m_member;
    }

private:
    result_type m_member;
};

} // namespace aux
} // namespace back
} // namespace di

#endif

