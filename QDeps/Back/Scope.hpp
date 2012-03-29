//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_SCOPE_HPP
#define QDEPS_BACK_SCOPE_HPP

#include <boost/mpl/apply.hpp>

namespace QDeps
{
namespace Back
{

template<typename TAllocator>
class Scope
{
public:
    template<typename T> struct ResultType
    {
        typedef typename boost::mpl::apply<TAllocator, T>::type type;
    };
};

} // namespace Back
} // namespace QDeps

#endif

