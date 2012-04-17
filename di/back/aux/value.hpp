//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_AUX_VALUE_HPP
#define DI_BACK_AUX_VALUE_HPP

#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/string.hpp>

namespace di
{
namespace back
{
namespace aux
{

namespace detail
{

template<typename Derived, typename = void> class HasValue
{
    struct Helper { static int value; };
    struct base : boost::mpl::if_<boost::is_arithmetic<Derived>, boost::mpl::void_, Derived>::type, Helper { };

    template<typename T> static boost::mpl::aux::no_tag  deduce(boost::non_type<const int*, &T::value>*);
    template<typename T> static boost::mpl::aux::yes_tag deduce(...);

public:
    static const bool value = sizeof(deduce<base>(0)) == sizeof(boost::mpl::aux::yes_tag);
};

}// namespace detail

template<typename, typename = void> class Value : public boost::mpl::false_ { };

template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
class Value< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> > : public boost::mpl::true_
{
public:
    typedef std::string result_type;

    inline static result_type create()
    {
        return boost::mpl::c_str< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> >::value;
    }
};

template<typename T>
class Value<T, typename boost::enable_if< detail::HasValue<T> >::type> : public boost::mpl::true_
{
public:
    typedef BOOST_TYPEOF_TPL(T::value) result_type;

    inline static result_type create()
    {
        return T::value;
    }
};

} // namespace aux
} // namespace back
} // namespace di

#endif

