//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CALL_STACK_HPP
#define BOOST_DI_FAKE_CALL_STACK_HPP


namespace boost {
namespace di {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct fake_call_stack
{
    typedef aux::mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> context_type;

    template<typename TContext, typename TCallStack>
    struct equal
      : aux::mpl::equal<
            aux::mpl::iterator_range<
                typename aux::mpl::advance<
                    typename aux::mpl::begin<TCallStack>::type
                  , typename aux::mpl::max<
                        aux::mpl::int_<0>
                      , aux::mpl::minus<
                            aux::mpl::size<TCallStack>
                          , aux::mpl::size<TContext>
                        >
                    >::type
                >::type
              , typename aux::mpl::end<TCallStack>::type
            >
          , TContext
        >
    { };

    template<typename TContext, typename TCallStack>
    struct apply_impl
        : aux::mpl::if_<
              aux::mpl::empty<TCallStack>
            , aux::mpl::int_<0>
            , aux::mpl::if_<
                  equal<TContext, TCallStack>
                , aux::mpl::size<TContext>
                , aux::mpl::int_<0>
              >
          >
    { };

    template<typename T>
    struct apply
        : apply_impl<context_type, typename T::call_stack>
    { };

};

} // namespace di
} // namespace boost

#endif

