//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace core {

template<
    class TParent = aux::none_t
  , class TInjector = aux::none_t
  , class TProvider = aux::none_t
  , class TRefs = aux::none_t
  , class TPolicies = aux::none_t
>
class any_type {
    template<class T, class TRef>
    using ref_type_t =
        std::conditional_t<std::is_same<T, aux::none_t>{}, T, TRef>;

    any_type& operator=(const any_type&) = delete;

    template<class T>
    using is_not_same_t = std::enable_if_t<
        !std::is_same<aux::make_plain_t<T>
      , aux::make_plain_t<TParent>>::value
    >;

public:
    any_type() noexcept { }

    any_type(const TInjector& creator
           , const TProvider& provider
           , TRefs& refs
           , const TPolicies& policies) noexcept
        : creator_(creator)
        , provider_(provider)
        , refs_(refs)
        , policies_(policies)
    { }

    template<class T, class = is_not_same_t<T>>
    operator const T&() const noexcept {
        return creator_.template
            create_impl<const T&, TParent>(provider_, refs_, policies_);
    }

    template<class T, class = is_not_same_t<T>>
    operator T&() const noexcept {
        return creator_.template
            create_impl<T&, TParent>(provider_, refs_, policies_);
    }

    template<class T, class = is_not_same_t<T>>
    operator T() noexcept {
        return creator_.template
            create_impl<T, TParent>(provider_, refs_, policies_);
    }

private:
    ref_type_t<TInjector, const TInjector&> creator_;
    ref_type_t<TProvider, const TProvider&> provider_;
    ref_type_t<TRefs, TRefs&> refs_;
    ref_type_t<TPolicies, const TPolicies&> policies_;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}} // namespace boost::di::core

#endif

