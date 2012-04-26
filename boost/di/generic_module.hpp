//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_GENERIC_MODULE_HPP
    #define BOOST_DI_GENERIC_MODULE_HPP

    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_PARAMS(                    \
            1                               \
          ,  BOOST_MPL_LIMIT_VECTOR_SIZE    \
          , "boost/di/generic_module.hpp"   \
        )                                   \
    )

    namespace boost {
    namespace di {

    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    class generic_module
        : public detail::module<mpl::vector<BOOST_DI_ARGS_MPL(T)> >
    {
        template<typename TInstance, typename T>
        struct is_same_instance
            : mpl::or_<
                  is_same<typename TInstance::name, T>
                , is_same<typename TInstance::value_type, T>
              >
        { };

        template<typename TSequence, typename T>
        struct find_instance_type
            : mpl::find_if<TSequence, is_same_instance<mpl::_1, T> >::type
        { };

        template<typename T, typename TExternals>
        struct disable_if_instance_not_found
            : disable_if<
                  is_same<
                      find_instance_type<TExternals, T>
                    , mpl::end<TExternals>
                  >
                , typename find_instance_type<TExternals, T>::type::derived
              >
        { };

    public:
        generic_module() { }

        #include BOOST_PP_ITERATE()

/*        template<typename T, typename TValue>*/
        //static typename disable_if_instance_not_found<T, externals>::type
        //set(TValue value) {
            //typedef typename find_instance_type<externals, T>::type annotation;
            //return typename annotation::derived(value);
        /*}*/
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_ARGS_TYPES(Args)>
    generic_module(BOOST_DI_ARGS(Args, args))
        : detail::module<mpl::vector<BOOST_DI_ARGS_MPL(T)> >(BOOST_DI_ARGS_FORWARD(args))
    { }

#endif

