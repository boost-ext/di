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
    class T = none_t
  , class TInjector = none_t
  , class TProvider = none_t
  , class TRefs = none_t
  , class TVisitors = none_t
>
class any_type {
    template<class TValueType, class TRefType>
    using ref_type_t = aux::conditional_t<
          std::is_same<TValueType, none_t>{}
        , TValueType
        , TRefType
      >;

    any_type& operator=(const any_type&) = delete;
    //any_type(const any_type&) = delete;

    template<class U>
    using is_not_same_t = aux::enable_if_t<!std::is_same<aux::make_plain_t<U>, aux::make_plain_t<T>>::value>;

public:
    using any = any_type;

    any_type() { }

    any_type(TInjector& creator
           , const TProvider& provider
           , TRefs& refs
           , const TVisitors& visitors)
        : creator_(creator)
        , provider_(provider)
        , refs_(refs)
        , visitors_(visitors)
    { }

    template<class U, class = is_not_same_t<U>>
    operator const U&() const {
        return creator_.template create<const U&, T>(provider_, refs_, visitors_);
    }

    template<class U, class = is_not_same_t<U>>
    operator U&() const {
        return creator_.template create<U&, T>(provider_, refs_, visitors_);
    }

    template<class U, class = is_not_same_t<U>>
    operator U() {
        return creator_.template create<U, T>(provider_, refs_, visitors_);
    }

private:
    ref_type_t<TInjector, TInjector&> creator_;
    ref_type_t<TProvider, const TProvider&> provider_;
    ref_type_t<TRefs, TRefs&> refs_;
    ref_type_t<TVisitors, const TVisitors&> visitors_;
};

} // namespace core
} // namespace di

/*template<*/
    //class T
  //, class TInjector
  //, class TProvider
  //, class TRefs
  //, class TVisitors
//>
//struct is_integral<
    //di::core::any_type<
        //T
      //, TInjector
      //, TProvider
      //, TRefs
      //, TVisitors
   //>
/*> : ::std::true_type { };*/

} // namespace boost

namespace std {

template<
    class T
  , class TInjector
  , class TProvider
  , class TRefs
  , class TVisitors
>
struct is_integral<
    boost::di::core::any_type<
        T
      , TInjector
      , TProvider
      , TRefs
      , TVisitors
    >
> : ::std::true_type { };

} // namespace std

#endif

