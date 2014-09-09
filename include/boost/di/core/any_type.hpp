//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/make_plain.hpp"

#include <boost/type_traits/is_integral.hpp>

namespace boost {
namespace di {
namespace core {

template<
    typename T = none_t
  , typename TCallStack = none_t
  , typename TCreator = none_t
  , typename TProvider = none_t
  , typename TDeps = none_t
  , typename TRefs = none_t
  , typename TVisitor = none_t
  , typename TPolicies = none_t
>
class any_type {
    any_type& operator=(const any_type&);

    template<typename TValueType, typename TRefType>
    using ref_type = std::conditional<
          std::is_same<TValueType, none_t>::value
        , TValueType
        , TRefType
      >;

public:
    using any = any_type;

    any_type() { }

    any_type(TCreator& creator
           , const TProvider& provider
           , TDeps& deps
           , TRefs& refs
           , const TVisitor& visitor
           , const TPolicies& policies)
        : creator_(creator)
        , provider_(provider)
        , deps_(deps)
        , refs_(refs)
        , visitor_(visitor)
        , policies_(policies)
    { }

    any_type(const any_type& other)
        : creator_(other.creator_)
        , provider_(other.provider_)
        , deps_(other.deps_)
        , refs_(other.refs_)
        , visitor_(other.visitor_)
        , policies_(other.policies_)
    { }

    template<
        typename U
      , typename = typename std::enable_if<
            !std::is_same<
                typename type_traits::make_plain<U>::type
              , typename type_traits::make_plain<T>::type
            >::value
        >::type
    >
    operator const U&() const {
        return creator_.template create<const U&, T, TCallStack>(
            provider_, deps_, refs_, visitor_, policies_
        );
    }

    template<
        typename U
      , typename = typename std::enable_if<
            !std::is_same<
                typename type_traits::make_plain<U>::type
              , typename type_traits::make_plain<T>::type
            >::value
        >::type
    >
    operator U&() const {
        return creator_.template create<U&, T, TCallStack>(
            provider_, deps_, refs_, visitor_, policies_
        );
    }

    template<typename U>
    operator aux::auto_ptr<U>&() {
        return creator_.template create<aux::auto_ptr<U>, T, TCallStack>(
            provider_, deps_, refs_, visitor_, policies_
        );
    }

    BOOST_DI_WKND(MSVC)(
        template<typename U>
        operator aux::unique_ptr<U>() {
            return creator_.template create<aux::unique_ptr<U>, T, TCallStack>(
                provider_, deps_, refs_, visitor_, policies_
            );
        }
    )

    BOOST_DI_WKND(NO_MSVC)(
        template<
            typename U
          , typename = typename std::enable_if<
                !std::is_same<
                    typename type_traits::make_plain<U>::type
                  , typename type_traits::make_plain<T>::type
                >::value
            >::type
        >
        operator U() {
            return creator_.template create<U, T, TCallStack>(
                provider_, deps_, refs_, visitor_, policies_
            );
        }
    )

private:
    typename ref_type<TCreator, TCreator&>::type creator_;
    typename ref_type<TProvider, const TProvider&>::type provider_;
    typename ref_type<TDeps, TDeps&>::type deps_;
    typename ref_type<TRefs, TRefs&>::type refs_;
    typename ref_type<TVisitor, const TVisitor&>::type visitor_;
    typename ref_type<TPolicies, const TPolicies&>::type policies_;
};

} // namespace core
} // namespace di

template<
    typename T
  , typename TCallStack
  , typename TCreator
  , typename TProvider
  , typename TDeps
  , typename TRefs
  , typename TVisitor
  , typename TPolicies
>
struct is_integral<
    di::core::any_type<
        T
      , TCallStack
      , TCreator
      , TProvider
      , TDeps
      , TRefs
      , TVisitor
      , TPolicies
   >
> : ::std::true_type { };

} // namespace boost

namespace std {

template<
    typename T
  , typename TCallStack
  , typename TCreator
  , typename TProvider
  , typename TDeps
  , typename TRefs
  , typename TVisitor
  , typename TPolicies
>
struct is_integral<
    boost::di::core::any_type<
        T
      , TCallStack
      , TCreator
      , TProvider
      , TDeps
      , TRefs
      , TVisitor
      , TPolicies
    >
> : ::std::true_type { };

} // namespace std

#endif

