//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/concepts/bind.hpp"
#include "boost/di/concepts/dependency.hpp"
#include "boost/di/concepts/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

#include <string>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/max.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/contains.hpp>

namespace boost {
namespace di {

template<typename TScope>
struct scope
    : concepts::scope<TScope, concepts::dependency>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
    : scope<scopes::deduce>::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct unique
    : scope<scopes::unique<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct shared
    : scope<scopes::shared<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct session
    : scope<scopes::session<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : concepts::bind<TExpected, TGiven, concepts::dependency>
{ };

template<int N>
struct bind_int
    : bind<int, mpl::int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

struct any_call_stack { };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class call_stack
{
public:
    typedef mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> context_type;

    template<typename TContext, typename TCallStack>
    struct apply_impl
        : mpl::if_<
              mpl::empty<TCallStack>
            , mpl::int_<0>
            , mpl::if_<
          mpl::or_<
                  mpl::equal<
                      mpl::iterator_range<
                          typename mpl::advance<
                              typename mpl::begin<TCallStack>::type
                            , typename mpl::max<
                                  mpl::int_<0>
                                , mpl::minus<
                                      mpl::size<TCallStack>
                                    , mpl::size<TContext>
                                  >
                              >::type
                          >::type
                        , typename mpl::end<TCallStack>::type
                      >
                    , TContext
                  >
                , mpl::contains<TCallStack, any_call_stack>
                  >
                , mpl::size<TContext>
                , mpl::int_<0>
              >
          >
    { };

    template<typename TSeq, typename V>
    static typename enable_if<mpl::empty<TSeq> >::type for_all(int&, const V&, int) { }

    template<typename TSeq, typename V>
    static typename disable_if<mpl::empty<TSeq> >::type for_all(int& result, const V& v, int i) {
        typedef typename mpl::front<TSeq>::type type;

        if (result != -1) {

            //std::cout << "BLAH: " << units::detail::demangle(typeid(type).name()) << units::detail::demangle(v[i]->name()) << std::endl;

            if (&typeid(type) == v[i]) {
                //std::cout <<"yay" << std::endl;
                result = i;
            } else {
                result = -1;
            }
        }

        for_all<typename mpl::pop_front<TSeq>::type>(result, v, i+1);
    }

public:
    template<typename, typename TCallStack, typename>
    struct apply
        : apply_impl<context_type, TCallStack>::type
    { };

    int operator()(const std::vector<const std::type_info*>& call_stack) const {
        //std::cout << units::detail::demangle(typeid(context_type).name()) << std::endl;
        //for (const auto& i : call_stack) {
            //std::cout << "Q:" << units::detail::demangle(i->name()) << std::endl;
        //}
        //std::cout << std::endl;

        if ((int)mpl::size<context_type>::value - (int)call_stack.size() >= 0) {
            return 0;
        }

        int result = 0;

        for_all<context_type>(result, call_stack, call_stack.size()- mpl::size<context_type>::value - 1);
        return result == -1 ? 0 : mpl::size<context_type>::value;
    }
};

} // namespace di
} // namespace boost

#endif

