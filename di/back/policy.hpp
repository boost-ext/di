//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_POLICY_HPP
#define DI_BACK_POLICY_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/void.hpp>
#include "di/back/aux/utility.hpp"
#include "di/back/policies/check_for_circular_dependencies.hpp"
#include "di/back/policies/check_for_creation_ownership.hpp"

namespace di
{
namespace back
{

namespace detail { class Policy { }; } // namespace detail

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
class Policy : detail::Policy
{
    typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> Seq;

    template<typename TDeps, typename T, typename TPolicy> struct AssertImpl
    {
        typedef typename TPolicy::template Assert<TDeps, typename aux::make_plain<T>::type>::type type;
    };

public:
    template<typename TDeps, typename T> struct Assert
        : boost::mpl::fold<Seq, boost::mpl::void_, AssertImpl<TDeps, T, boost::mpl::_2> >::type
    {
        typedef void type;
    };
};

} // namespace back

template<typename TDefault>
struct Defaults<back::detail::Policy, TDefault>
{
    typedef back::Policy
    <
        back::Policies::CheckForCircularDependencies,
        back::Policies::CheckForCreationOwnership
    >
    type;
};

} // namespace di

#endif

