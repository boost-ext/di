//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_FACTORY_HPP
#define QDEPS_BACK_FACTORY_HPP

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include "QPool/Pool.hpp"
#include "QDeps/Back/Policy.hpp"
#include "QDeps/Back/Aux/Utility.hpp"
#include "QDeps/Back/Aux/Dependency.hpp"
#include "QDeps/Back/Detail/Converter.hpp"
#include "QDeps/Back/Detail/Creator.hpp"
#include "QDeps/Back/Detail/Visitor.hpp"

namespace QDeps
{
namespace Back
{

template
<
    typename TDeps,
    typename TPool = const QPool::Pool<>,
    typename TPolices = Policy<>,
    template<typename> class TConverter = Detail::Converter,
    template<typename = TDeps, typename = TPool> class TCreator = Detail::Creator,
    template<typename = TDeps> class TVisitor = Detail::Visitor
>
class Factory
{
    typedef typename TPolices::template Assert<TDeps>::type PolicyType;

    struct Entries
        : boost::mpl::inherit_linearly<TDeps, boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2> >::type
    { };

public:
    explicit Factory(TPool& p_pool = TPool())
        : m_pool(p_pool)
    { }

    template<typename T> T create()
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename Aux::MakePlain<T>::type PlainType;
        return TConverter<T>::execute(TCreator<>::template execute<PlainType, EmptyCallStack>(m_entries, m_pool));
    }

    template<typename T, typename Visitor> void visit(const Visitor& p_visitor)
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename Aux::MakePlain<T>::type PlainType;
        TVisitor<>::template execute<PlainType, T, EmptyCallStack>(p_visitor);
    }

private:
    TPool& m_pool;
    Entries m_entries;
};

} // namespace Back
} // namespace QDeps

#endif

