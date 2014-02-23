//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_WHEN_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_WHEN_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/max_element.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

template<typename TContext>
class when
{
    template<typename TBind, typename T, typename TCallStack, typename TScope>
    struct apply_bind
        : TBind::template apply<T, TCallStack, TScope>::type
    { };

    template<typename TSeq, typename V>
    static typename enable_if<mpl::empty<TSeq> >::type for_all(int&, const V&) { }

    template<typename TSeq, typename V>
    static typename disable_if<mpl::empty<TSeq> >::type for_all(int& max, const V& v) {
        typedef typename mpl::front<TSeq>::type type;
        int value = type()(v);

        if (value > max) {
            max = value;
        }

        for_all<typename mpl::pop_front<TSeq>::type>(max, v);
    }

public:
    template<typename T>
    struct context
    {
        typedef typename T::context_type type;
    };
    typedef typename mpl::fold<
        TContext
      ,  mpl::vector0<>
      , mpl::if_<
            mpl::greater<
                mpl::size<context<mpl::_2> >
              , mpl::size<mpl::_1>
            >
          , context<mpl::_2>
          , mpl::_1
        >
    >::type max;

    template<typename T, typename TCallStack, typename TScope>
    struct apply
        : mpl::if_<
              mpl::empty<TContext>
            , mpl::int_<1>
            , typename mpl::deref<
                  mpl::max_element<
                      mpl::transform_view<TContext, apply_bind<mpl::_1, T, TCallStack, TScope> >
                  >
              >::type
          >::type
    { };

    int operator()(const std::type_info*, const std::type_info*, const std::vector<const std::type_info*>& call_stack) const {
        if (mpl::empty<TContext>::value) {
            return 1;
        }

        int max = 0;
        for_all<TContext>(max, call_stack);

        return max;
    }
};

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

#endif

