//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POOL_HPP
#define BOOST_DI_CORE_POOL_HPP

#include "boost/di/aux_/mpl.hpp"

#include <type_traits>

namespace boost {
namespace di {
namespace core {

struct init { };

template<typename = type_list<>>
class pool;

template<typename... TArgs>
class pool<type_list<TArgs...>> : public TArgs... {
public:
    using type = pool;

    template<typename... Ts>
    explicit pool(const Ts&... args)
        : Ts(args)...
    { }

    template<typename TPool>
    pool(const init&, const TPool& p)
        : pool(get<TArgs>(p)...)
    { }

    template<typename T>
    const T& get() const {
        return static_cast<const T&>(*this);
    }

private:
    template<typename T, typename TPool>
    typename std::enable_if<std::is_base_of<T, pool>::value && std::is_base_of<T, TPool>::value, T>::type
    get(const TPool& p) {
        return p.template get<T>();
    }

    template<typename T, typename TPool>
    typename std::enable_if<std::is_base_of<T, pool>::value && !std::is_base_of<T, TPool>::value, T>::type
    get(const TPool&) {
        return T();
    }

    template<typename T, typename TPool>
    typename std::enable_if<!std::is_base_of<T, pool>::value, const T&>::type
    get(const TPool&) {
        return T();
    }
};

} // namespace core
} // namespace di
} // namespace boost

#endif

