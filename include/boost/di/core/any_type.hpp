//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost {
namespace di {
namespace core {

template<
    typename T = none_t
  , typename TInjector = none_t
  , typename TProvider = none_t
  , typename TRefs = none_t
  , typename TVisitor = none_t
  , typename TPolicies = none_t
>
class any_type {
    template<typename TValueType, typename TRefType>
    using ref_type_t = aux::conditional_t<
          std::is_same<TValueType, none_t>{}
        , TValueType
        , TRefType
      >;

    any_type& operator=(const any_type&) = delete;
    //any_type(const any_type&) = delete;

    template<typename U>
    using is_not_same_t = aux::enable_if_t<!std::is_same<aux::make_plain_t<U>, aux::make_plain_t<T>>::value>;

public:
    using any = any_type;

    any_type() { }

    any_type(TInjector& creator
           , const TProvider& provider
           , TRefs& refs
           , const TVisitor& visitor
           , const TPolicies& policies)
        : creator_(creator)
        , provider_(provider)
        , refs_(refs)
        , visitor_(visitor)
        , policies_(policies)
    { }

    template<typename U, typename = is_not_same_t<U>>
    operator const U&() const {
        return creator_.template create<const U&, T>(provider_, refs_, visitor_, policies_);
    }

    template<typename U, typename = is_not_same_t<U>>
    operator U&() const {
        return creator_.template create<U&, T>(provider_, refs_, visitor_, policies_);
    }

    template<typename U, typename = is_not_same_t<U>>
    operator U() {
        return creator_.template create<U, T>(provider_, refs_, visitor_, policies_);
    }

private:
    ref_type_t<TInjector, TInjector&> creator_;
    ref_type_t<TProvider, const TProvider&> provider_;
    ref_type_t<TRefs, TRefs&> refs_;
    ref_type_t<TVisitor, const TVisitor&> visitor_;
    ref_type_t<TPolicies, const TPolicies&> policies_;
};

} // namespace core
} // namespace di

/*template<*/
    //typename T
  //, typename TInjector
  //, typename TProvider
  //, typename TRefs
  //, typename TVisitor
  //, typename TPolicies
//>
//struct is_integral<
    //di::core::any_type<
        //T
      //, TInjector
      //, TProvider
      //, TRefs
      //, TVisitor
      //, TPolicies
   //>
/*> : ::std::true_type { };*/

} // namespace boost

namespace std {

template<
    typename T
  , typename TInjector
  , typename TProvider
  , typename TRefs
  , typename TVisitor
  , typename TPolicies
>
struct is_integral<
    boost::di::core::any_type<
        T
      , TInjector
      , TProvider
      , TRefs
      , TVisitor
      , TPolicies
    >
> : ::std::true_type { };

} // namespace std

#endif

