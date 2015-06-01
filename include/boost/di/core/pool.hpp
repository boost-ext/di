//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POOL_HPP
#define BOOST_DI_CORE_POOL_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class = aux::type_list<>>
class pool;

template<class... TArgs>
using pool_t = pool<aux::type_list<TArgs...>>;

template<class... TArgs>
class pool<aux::type_list<TArgs...>> : public TArgs... {
public:
    template<class... Ts>
    explicit pool(const Ts&... args) noexcept
        : Ts(args)...
    { }

    template<class... Ts, class TPool>
    pool(const aux::type_list<Ts...>&, const TPool& p) noexcept
        : pool(static_cast<const Ts&>(p)...)
    { }
};

}}}} // boost::di::v1::core

#endif

