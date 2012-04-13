//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_VALUE_HPP
#define QDEPS_BACK_AUX_VALUE_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/string.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

template<typename> class Value : public boost::mpl::false_ { };

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

template<int N>
class Value< boost::mpl::int_<N> > : public boost::mpl::true_
{
public:
    typedef int ResultType;

    inline static ResultType create()
    {
        return N;
    }
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

