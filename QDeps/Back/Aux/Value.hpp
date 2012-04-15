//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_VALUE_HPP
#define QDEPS_BACK_AUX_VALUE_HPP

#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/string.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

namespace Detail
{

template<typename Derived, typename = void> class HasValue
{
    struct Helper { static int value; };
    struct Base : boost::mpl::if_<boost::is_arithmetic<Derived>, boost::mpl::void_, Derived>::type, Helper { };

    template<typename T> static boost::mpl::aux::no_tag  deduce(boost::non_type<const int*, &T::value>*);
    template<typename T> static boost::mpl::aux::yes_tag deduce(...);

public:
    static const bool value = sizeof(deduce<Base>(0)) == sizeof(boost::mpl::aux::yes_tag);
};

}// namespace Detail

template<typename, typename = void> class Value : public boost::mpl::false_ { };

template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
class Value< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> > : public boost::mpl::true_
{
public:
    typedef std::string ResultType;

    inline static ResultType create()
    {
        return boost::mpl::c_str< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> >::value;
    }
};

template<typename T>
class Value<T, typename boost::enable_if< Detail::HasValue<T> >::type> : public boost::mpl::true_
{
public:
    typedef BOOST_TYPEOF_TPL(T::value) ResultType;

    inline static ResultType create()
    {
        return T::value;
    }
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

