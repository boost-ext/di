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
    typename TPool = aux::Pool<>,
    typename TPolices = Policy<>,
    template<typename, typename> class TConverter = detail::Converter,
    template<typename = TDeps, typename = TPool> class TCreator = detail::Creator,
    template<typename = TDeps> class TVisitor = detail::Visitor
>
class Factory
{
    struct Entries
        : boost::mpl::inherit_linearly<TDeps, boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2> >::type
    { };

public:
    explicit Factory(const TPool& p_pool = TPool())
        : m_pool(p_pool)
    { }

    template<typename T> T create()
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename TPolices::template Assert<TDeps, T>::type Polices;
        return TConverter<Scopes::PerRequest, T>::execute(TCreator<>::template execute<T, EmptyCallStack>(m_entries, m_pool));
    }

    template<typename T, typename Visitor> void visit(const Visitor& p_visitor)
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename TPolices::template Assert<TDeps, T>::type Polices;
        TVisitor<>::template execute<T, EmptyCallStack>(p_visitor);
    }

private:
    const TPool& m_pool;
    Entries m_entries;
};

} // namespace back
} // namespace di

#endif

