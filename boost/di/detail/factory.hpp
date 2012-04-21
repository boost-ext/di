//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_FACTORY_HPP
#define BOOST_DI_DETAIL_FACTORY_HPP

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include "boost/di/aux/pool.hpp"
#include "boost/di/aux/utility.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/detail/converter.hpp"
#include "boost/di/detail/creator.hpp"
#include "boost/di/detail/visitor.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/policy.hpp"

namespace boost {
namespace di {
namespace detail {

template<
    typename TDeps
  , typename TPool = aux::pool<>
  , typename TPolicies = di::policy<>
  , template<typename, typename> class TConverter = converter
  , template<typename = TDeps, typename = TPool> class TCreator = creator
  , template<typename = TDeps> class TVisitor = visitor
>
class factory
{
    struct entries
        : mpl::inherit_linearly<TDeps, mpl::inherit<mpl::_1, mpl::_2> >::type
    { };

    typedef typename mpl::if_<
        mpl::empty<typename TPool::sequence>
      , TPool
      , const TPool&
    >::type pool_t;

public:
    factory() { }

    explicit factory(const TPool& pool)
        : pool_(pool)
    { }

    template<typename T>
    T create() {
        typedef mpl::vector0<> empty_call_stack;
        typedef typename TPolicies::template verify<TDeps, T>::type policies;
        return TConverter<scopes::per_request, T>::execute(
            TCreator<>::template execute<T, empty_call_stack>(entries_, pool_));
    }

    template<typename T, typename Visitor>
    void visit(const Visitor& visitor) {
        typedef mpl::vector0<> empty_call_stack;
        typedef typename TPolicies::template verify<TDeps, T>::type policies;
        TVisitor<>::template execute<T, empty_call_stack>(visitor);
    }

private:
    pool_t pool_;
    entries entries_;
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

