//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POOL_HPP
#define BOOST_DI_CORE_POOL_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace core {

struct init { };

template<class = aux::type_list<>>
class pool;

template<class... TArgs>
class pool<aux::type_list<TArgs...>> : public TArgs... {
    template<class T, class TPool>
    using is_base_of_pool = std::integral_constant<
        bool
      , std::is_base_of<T, pool>{} && std::is_base_of<T, TPool>{}
    >;

public:
    template<class... Ts>
    explicit pool(const Ts&... args) noexcept
        : Ts(args)...
    { }

    template<class TPool>
    pool(const init&, const TPool& p) noexcept
        : pool(get<TArgs>(p, is_base_of_pool<TArgs, TPool>{})...)
    { }

    template<class T>
    inline const T& get() const noexcept {
        return static_cast<const T&>(*this);
    }

private:
    template<class T, class TPool>
    inline const T& get(const TPool& p, const std::true_type&) const noexcept { 
        return p.template get<T>();
    }

    template<class T, class TPool>
    inline T get(const TPool&, const std::false_type&) const noexcept {
        return {};
    }
};

}}} // boost::di::core

#endif

