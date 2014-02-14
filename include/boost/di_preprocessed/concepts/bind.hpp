#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include "boost/di_preprocessed/aux_/common.hpp"
#include "boost/di_preprocessed/concepts/type_traits/is_req_type.hpp"
#include "boost/di_preprocessed/concepts/type_traits/is_req_name.hpp"
#include "boost/di_preprocessed/concepts/type_traits/is_req_call.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/times.hpp>

namespace boost {
namespace di {
namespace concepts {

template<
    typename TExpected
  , typename TGiven
  , template<
        typename
      , typename
      , typename
      , typename
    > class TDependency
>
struct bind
    : TDependency<
          mpl::_1
        , TExpected
        , TGiven
        , typename mpl::lambda<
              type_traits::is_req_type<TExpected>
          >::type
      >
{
    template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
    struct when
        : TDependency<
              mpl::_1
            , TExpected
            , TGiven
            , typename mpl::lambda<
                  mpl::times<
                      type_traits::is_req_type<TExpected>
                    , type_traits::is_req_call<mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19> >
                  >
              >::type
          >
    {
        template<typename TName>
        struct named
            : TDependency<
                  mpl::_1
                , TExpected
                , TGiven
                , typename mpl::lambda<
                      mpl::times<
                          mpl::times<
                              type_traits::is_req_type<TExpected>
                            , type_traits::is_req_name<TName>
                          >
                        , type_traits::is_req_call<mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19> >
                      >
                >::type
              >
        { };
    };

    template<typename TName>
    struct named
        : TDependency<
              mpl::_1
            , TExpected
            , TGiven
            , typename mpl::lambda<
                  mpl::times<
                      type_traits::is_req_type<TExpected>
                    , type_traits::is_req_name<TName>
                  >
            >::type
          >
    {
        template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
        struct when
            : TDependency<
                  mpl::_1
                , TExpected
                , TGiven
                , typename mpl::lambda<
                      mpl::times<
                          mpl::times<
                              type_traits::is_req_type<TExpected>
                            , type_traits::is_req_name<TName>
                          >
                        , type_traits::is_req_call<mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19> >
                      >
                >::type
              >
        { };
    };
};

}
}
}

#endif

