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
struct get_value_type
{
    typedef T type;
};

template<>
struct get_value_type<std::string, void>
{
    typedef std::string type;
};

template<typename T>
struct get_value_type<T, typename boost::enable_if<has_value_type<T> >::type>
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
class instance
{
public:
    typedef T value_type;
    typedef boost::variant<const T&, T&, boost::shared_ptr<T> > result_type;

    explicit instance(const T& member)
        : member_(member)
    { }

    explicit instance(T& member)
        : member_(member)
    { }

    explicit instance(boost::shared_ptr<T> member)
        : member_(member)
    { }

    result_type get() const
    {
        return member_;
    }

private:
    result_type member_;
};

template
<
    typename T,
    typename TContext
>
class instance
    <
        T, TContext,
        typename boost::enable_if
        <
            boost::mpl::or_
            <
                boost::is_same<typename detail::get_value_type<T>::type, std::string>,
                boost::is_pod<typename detail::get_value_type<T>::type>
            >
        >::type
    >
{
public:
    typedef T value_type;
    typedef typename detail::get_value_type<T>::type result_type;

    explicit instance(result_type member)
        : member_(member)
    { }

    result_type get() const
    {
        return member_;
    }

private:
    result_type member_;
};

} // namespace aux
} // namespace back
} // namespace di

#endif

