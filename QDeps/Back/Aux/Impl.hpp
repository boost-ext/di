//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_IMPL_HPP
#define QDEPS_BACK_AUX_IMPL_HPP

namespace QDeps
{
namespace Back
{
namespace Aux
{

template
<
    typename TScope,
    typename TIf,
    typename TImpl
>
struct Impl
{
    //TODO asserts about types -> no shared_ptr, etc.
    typedef TScope Scope;
    typedef TIf Interface;
    typedef TImpl Implementation;
    typedef TIf Dependency;
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

