//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_DI_FRONT_BASE_ANNOTATE_HPP
#define DI_DI_FRONT_BASE_ANNOTATE_HPP

namespace di
{
namespace front
{
namespace base
{
namespace aux
{

template<typename TDerived>
struct annotate
{
    template<typename TName = void>
    struct with : TDerived
    {
        typedef TDerived derived;
        typedef TName name;
    };
};

} // namespace aux
} // namespace base
} // namespace front
} // namespace di

#endif

