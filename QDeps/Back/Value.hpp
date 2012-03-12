//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_QDEPS_BACK_VALUE_HPP
#define QDEPS_QDEPS_BACK_VALUE_HPP

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/contains.hpp>

namespace QDeps
{
namespace Back
{

template<typename T>
struct Value : boost::mpl::true_
{
    template<typename TAttr, typename TPool, typename TScope>
    static boost::shared_ptr<TAttr> get(TPool& p_pool, TScope&, typename boost::enable_if< boost::mpl::contains<typename TPool::Seq, TAttr> >::type* = 0)
    {
        return p_pool.template get<TAttr>();
    }

    template<typename TAttr, typename TPool, typename TScope>
    static boost::shared_ptr<TAttr> get(TPool&, TScope& p_scope, typename boost::disable_if< boost::mpl::contains<typename TPool::Seq, TAttr> >::type* = 0)
    {
        return p_scope.template create<TAttr>();
    }
};

template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
struct Value< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> > : boost::mpl::false_
{
    template<typename TAttr, typename TPool, typename TScope> static boost::shared_ptr<TAttr> get(TPool&, TScope&)
    {
        return boost::make_shared<TAttr>(
            boost::mpl::c_str< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> >::value
        );
    }
};

template<int N>
struct Value<boost::mpl::int_<N> > : boost::mpl::false_
{
    template<typename TAttr, typename TPool, typename TScope> static boost::shared_ptr<TAttr> get(TPool&, TScope&)
    {
        return boost::make_shared<TAttr>(N);
    }
};

} // namespace Back
} // namespace QDeps

#endif

