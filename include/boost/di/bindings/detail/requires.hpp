//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_DETAIL_REQUIRES_HPP
#define BOOST_DI_BINDINGS_DETAIL_REQUIRES_HPP

#include "boost/di/aux_/config.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename... Ts>
class requires_
{
    template<
        typename TBind
      , typename T
    >
    struct apply_bind
        : TBind::template apply<T>::type
    { };

public:
    template<
        typename T
      , typename TMultiplicationFactor = aux::mpl::integral_c<long, 10>
    >
    struct apply
        : aux::mpl::second<
              typename aux::mpl::fold<
                  aux::mpl::vector<Ts...>
                , aux::mpl::pair<aux::mpl::integral_c<long, 1>, aux::mpl::integral_c<long, 1> >
                , aux::mpl::pair<
                      aux::mpl::times<
                          aux::mpl::first<aux::mpl::_1>
                        , TMultiplicationFactor
                      >
                    , aux::mpl::times<
                          aux::mpl::first<aux::mpl::_1>
                        , aux::mpl::second<aux::mpl::_1>
                        , apply_bind<aux::mpl::_2, T>
                      >
                  >
              >::type
          >
    { };
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

#endif

