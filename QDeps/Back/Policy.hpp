//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_POLICY_HPP
#define QDEPS_BACK_POLICY_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/for_each.hpp>
#include "QDeps/Back/Aux/Utility.hpp"

namespace QDeps
{
namespace Back
{

namespace Detail
{
class Policy { };
} // namespace Detail

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
class Policy : Detail::Policy
{
    typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> Seq;

    template<typename TDeps, typename TPolicy> struct VerifyImpl
    {
        typedef typename TPolicy::template Verify<TDeps> type;
    };

    template<typename TVisit, typename TPlainType, typename T, typename TCallStack>
    class VisitImpl
    {
    public:
        template<typename TPolicy> void operator()(const TPolicy&) const
        {
            TVisit::template execute<TPlainType, T, TCallStack>(TPolicy());
        }
    };

public:
    template<typename TDeps> struct Verify : boost::mpl::fold
        <
            Seq,
            boost::mpl::void_,
            boost::mpl::if_
            <
                Aux::Detail::HasCallOperator<boost::mpl::_2>,
                boost::mpl::void_,
                VerifyImpl<TDeps, boost::mpl::_2>
            >
        >::type
    {
        typedef void type;
    };

    template<typename TVisit, typename TPlainType, typename T, typename TCallStack>
    static void verify()
    {
        typedef typename boost::mpl::filter_view<Seq, Aux::Detail::HasCallOperator<boost::mpl::_> >::type Visitors;
        boost::mpl::for_each<Visitors>(VisitImpl<TVisit, TPlainType, T, TCallStack>());
    }
};

} // namespace Back
} // namespace QDeps

#endif

