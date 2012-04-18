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
#include <boost/mpl/transform.hpp>
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

template
<
    typename TDeps,
    typename TPool = aux::pool<>,
    typename TPolicies = di::policy<>,
    template<typename, typename> class TConverter = converter,
    template<typename = TDeps, typename = TPool> class TCreator = creator,
    template<typename = TDeps> class TVisitor = visitor
>
class factory
{
    struct entries
        : boost::mpl::inherit_linearly<TDeps, boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2> >::type
    { };

public:
    explicit factory(const TPool& pool = TPool())
        : pool_(pool)
    { }

    template<typename T>
    T create()
    {
        typedef boost::mpl::vector0<> empty_call_stack;
        typedef typename TPolicies::template verify<TDeps, T>::type policies;
        return TConverter<scopes::per_request, T>::execute(
            TCreator<>::template execute<T, empty_call_stack>(m_entries, pool_));
    }

    template<typename T, typename Visitor>
    void visit(const Visitor& visitor)
    {
        typedef boost::mpl::vector0<> empty_call_stack;
        typedef typename TPolicies::template verify<TDeps, T>::type policies;
        TVisitor<>::template execute<T, empty_call_stack>(visitor);
    }

private:
    const TPool& pool_;
    entries m_entries;
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

