//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/type_traits/has_ctor.hpp"
#include "boost/di/type_traits/has_injector.hpp"
#include "boost/di/type_traits/function_traits.hpp"

#include <string>
#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

template < class T, class R >
struct normalize;

template < class... TTypes, class X >
struct normalize< aux::mpl::vector< TTypes... >, X >
   : aux::mpl::vector< TTypes..., X >
{ };

template<typename TSeq>
struct normalize_vector
    : aux::mpl::fold<
          TSeq
        , aux::mpl::vector<>
        , normalize<aux::mpl::arg<0>, aux::mpl::arg<1> >
      >::type
{ };

template<typename T>
struct ctor_traits
          : aux::mpl::second<typename aux::mpl::fold<
              aux::mpl::range_c<int, 0, BOOST_DI_CFG_CTOR_LIMIT_SIZE>
            , aux::mpl::pair<
                  aux::mpl::vector<core::any_type<T> >
                , aux::mpl::vector<>
              >
            , aux::mpl::if_<
                  type_traits::has_ctor<T, normalize_vector<aux::mpl::first<aux::mpl::arg<0> > > >
                , aux::mpl::pair<
                      aux::mpl::push_back<aux::mpl::first<aux::mpl::arg<0> >, core::any_type<T> >
                    , aux::mpl::first<aux::mpl::arg<0> >
                  >
                , aux::mpl::pair<
                      aux::mpl::push_back<aux::mpl::first<aux::mpl::arg<0> >, core::any_type<T> >
                    , aux::mpl::second<aux::mpl::arg<0> >
                  >
              >
          >::type >
{ };

template<typename T>
struct ctor_traits<std::basic_string<T> > // basic_string ctors are ambiguous
{
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

template<typename T, typename = void>
struct ctor_traits
    : di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T> > >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<T> >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&T::BOOST_DI_INJECTOR)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

