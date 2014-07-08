//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_CORE_ALLOCATOR_HPP
    #define BOOST_DI_CORE_ALLOCATOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/type_traits/has_value.hpp"

    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {

    namespace mpl {
        struct string_tag;
        template<typename> struct c_str;
    } // namespace mpl

    namespace di {
    namespace core {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)

    template<typename, typename = void>
    struct is_mpl_string
        : mpl::false_
    { };

    template<typename T>
    struct is_mpl_string<T, typename enable_if<has_tag<T> >::type>
        : is_same<mpl::string_tag, typename T::tag>
    { };

    template<typename T>
    struct is_explicit
        : mpl::or_<
              type_traits::has_value<T>
            , is_mpl_string<T>
          >
    { };

    class allocator
    {
    public:
        template<typename TExpected, typename TGiven>
        typename disable_if<is_explicit<TGiven>, TExpected*>::type
        allocate() const {
            return new TGiven();
        }

        template<typename TExpected, typename TGiven>
        typename enable_if<type_traits::has_value<TGiven>, TExpected*>::type
        allocate() const {
            return new TExpected(TGiven::value);
        }

        template<typename TExpected, typename TGiven>
        typename enable_if<is_mpl_string<TGiven>, TExpected*>::type
        allocate() const {
            return new TExpected(mpl::c_str<TGiven>::value);
        }

        #define BOOST_PP_FILENAME_1 "boost/di/core/allocator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM_(1)
        #include BOOST_PP_ITERATE()
    };

    } // namespace core
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES_(TArgs)>
    TExpected* allocate(BOOST_DI_FORWARD_ARGS(TArgs, args)) const {
        return new TGiven(BOOST_DI_ARGS_FORWARD(args));
    }

#endif

