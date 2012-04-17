//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_FACTORY_HPP
#define DI_BACK_FACTORY_HPP

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include "di/back/aux/pool.hpp"
#include "di/back/policy.hpp"
#include "di/back/aux/utility.hpp"
#include "di/back/aux/dependency.hpp"
#include "di/back/scopes/per_request.hpp"
#include "di/back/detail/converter.hpp"
#include "di/back/detail/creator.hpp"
#include "di/back/detail/visitor.hpp"

namespace di
{
namespace back
{

template
<
    typename TDeps,
    typename TPool = aux::pool<>,
    typename Tpolicies = policy<>,
    template<typename, typename> class TConverter = detail::converter,
    template<typename = TDeps, typename = TPool> class TCreator = detail::creator,
    template<typename = TDeps> class TVisitor = detail::visitor
>
class factory
{
    struct entries
        : boost::mpl::inherit_linearly<TDeps, boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2> >::type
    { };

public:
    explicit factory(const TPool& p_pool = TPool())
        : m_pool(p_pool)
    { }

    template<typename T> T create()
    {
        typedef boost::mpl::vector0<> empty_call_stack;
        typedef typename Tpolicies::template verify<TDeps, T>::type polices;
        return TConverter<scopes::per_request, T>::execute(TCreator<>::template execute<T, empty_call_stack>(m_entries, m_pool));
    }

    template<typename T, typename Visitor> void visit(const Visitor& p_visitor)
    {
        typedef boost::mpl::vector0<> empty_call_stack;
        typedef typename Tpolicies::template verify<TDeps, T>::type polices;
        TVisitor<>::template execute<T, empty_call_stack>(p_visitor);
    }

private:
    const TPool& m_pool;
    entries m_entries;
};

} // namespace back
} // namespace di

#endif

