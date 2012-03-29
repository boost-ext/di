//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_QDEPS_BACK_VALUE_HPP
#define QDEPS_QDEPS_BACK_VALUE_HPP

#include <string>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/string.hpp>

namespace QDeps
{
namespace Back
{

template<typename> struct Value : boost::mpl::false_
{
    template<typename T> static T create();
};

template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
struct Value< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> > : boost::mpl::true_
{
    template<typename T> static boost::shared_ptr<T> create()
    {
        return boost::make_shared<T>(
            boost::mpl::c_str< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> >::value
        );
    }
};

template<int N>
struct Value< boost::mpl::int_<N> > : boost::mpl::true_
{
    template<typename T> static boost::shared_ptr<T> create()
    {
        return boost::make_shared<T>(N);
    }
};

} // namespace Back
} // namespace QDeps

#endif

