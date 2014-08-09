//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_DETAIL_WHEN_HPP
#define BOOST_DI_BINDINGS_DETAIL_WHEN_HPP

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename... Ts>
class when_
{
    template<
        typename TBind
      , typename T
    >
    struct apply_bind
        : TBind::template apply<T>::type
    { };

public:
    template<typename T>
    struct apply
        : aux::mpl::if_<
              aux::mpl::empty<aux::mpl::vector<Ts...>>
            , aux::mpl::int_<1>
            , typename aux::mpl::deref<
                  aux::mpl::max_element<
                      aux::mpl::transform_view<
                          aux::mpl::vector<Ts...>
                        , apply_bind<aux::mpl::_1, T>
                      >
                  >
              >::type
          >::type
    { };
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

#endif

