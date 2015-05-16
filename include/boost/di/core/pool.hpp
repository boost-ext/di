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

struct init { };
template<class>
struct init2 { };

template<class = aux::type_list<>>
class pool;

template<class... TArgs>
class pool<aux::type_list<TArgs...>> : public TArgs... {
    template<class T, class = void>
    struct get_ {
        using type = aux::type_list<>;
    };

    template<class T>
    struct get_<T, std::enable_if_t<!aux::is_braces_constructible<typename T::scope_t>::value>> {
        using type = aux::type_list<T>;
    };

    using blah = aux::join_t<typename get_<TArgs>::type...>;

public:
    template<class... Ts>
    explicit pool(const Ts&... args) noexcept
        : Ts(args)...
    { }

    template<class TPool>
    pool(const init&, const TPool& p) noexcept
        : pool(init2<blah>{}, p)
    { }

    template<class... Ts, class TPool>
    pool(const init2<aux::type_list<Ts...>>&, const TPool& p) noexcept
        : pool(static_cast<const Ts&>(p)...)
    { }

    template<class T>
    inline const T& get() const noexcept {
        return static_cast<const T&>(*this);
    }
};

}}}} // boost::di::v1::core

#endif

