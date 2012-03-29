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
#include "QPool/Pool.hpp"
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Convert.hpp"
#include "QDeps/Back/Create.hpp"
#include "QDeps/Back/Dependency.hpp"

namespace QDeps
{
namespace Back
{

template
<
    typename TDeps,
    typename TPool = const QPool::Pool<>,
    template<typename> class TConvert = Convert,
    template<typename = TDeps, typename = TPool> class TCreate = Create
>
class Factory
{
    struct Entries : boost::mpl::inherit_linearly
        <
            TDeps,
            boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };

public:
    explicit Factory(TPool& p_pool = TPool())
        : m_pool(p_pool)
    { }

    template<typename T> T create()
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename MakePlain<T>::type PlainType;
        typedef typename TCreate<>::type Create;

        return TConvert<T>::execute(Create::template execute<PlainType, EmptyCallStack>(m_entries, m_pool));
    }

private:
    TPool& m_pool;
    Entries m_entries;
};

} // namespace Back
} // namespace QDeps

#endif

