//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POOL_HPP
#define BOOST_DI_CORE_POOL_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_MPL_HAS_XXX_TRAIT_DEF(types)

class init { };

struct never {
    template<typename>
    struct apply : std::false_type { };
};

template<typename = type_list<>, typename = never>
class pool;

template<typename TIgnore, typename... TArgs>
class pool<type_list<TArgs...>, TIgnore> : public TArgs...  {
    template<typename T, typename = void>
    struct pool_type {
        using type = type_list<T>;
    };

    template<typename T>
    struct pool_type<T, typename std::enable_if<has_types<T>::value>::type> {
        using type = typename T::types;
    };

    template<typename T>
    struct pool_type<T, typename std::enable_if<TIgnore::template apply<T>::value>::type> {
        using type = type_list<>;
    };

public:
    using type = pool;
    using types = typename join<typename pool_type<TArgs>::type...>::type;

    template<typename... T>
    explicit pool(const T&... args)
        : T(args)...
    { }

    template<typename TPool>
    pool(const TPool& p, const init&)
        : pool(p, typename TPool::types(), init())
    { }

    template<typename T>
    const T& get() const {
        return static_cast<const T&>(*this);
    }

private:
    template<typename TPool, typename... T>
    pool(const TPool& p, const type_list<T...>&, const init&)
        : T(p.template get<T>())...
    { }
};

} // namespace core
} // namespace di
} // namespace boost

#endif

