//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include "boost/di/type_traits/ctor_traits.hpp"

    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {

    struct none { };

    namespace detail {

    template<typename TBinder>
    class creator
    {
        template<typename TDependency>
        struct ctor
            : type_traits::ctor_traits<typename TDependency::given>::type
        { };

        template<typename T, typename TCallStack>
        struct binder
            : TBinder::template get_dependency<T, TCallStack>::type
        { };

        template<
            typename TCallStack
          , typename TDeps
          , typename TCleanup
        >
        struct eager_creator
        {
            eager_creator(TDeps& deps, TCleanup& cleanup)
                : deps_(deps), cleanup_(cleanup)
            { }

            template<typename T>
            operator T() const {
                return creator::execute<T, TCallStack>(deps_, cleanup_)(boost::type<T>());
            }

            TDeps& deps_;
            TCleanup& cleanup_;
        };

    public:
        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
        >
        static eager_creator<TCallStack, TDeps, TCleanup>
        execute(TDeps& deps, TCleanup& cleanup, typename enable_if<is_same<T, none>>::type* = 0) {
            return eager_creator<TCallStack, TDeps, TCleanup>(deps, cleanup);
        }

        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
        >
        static typename binder<T, TCallStack>::result_type
        execute(TDeps& deps, TCleanup& cleanup, typename disable_if<is_same<T, none>>::type* = 0) {
            return execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , binder<T, TCallStack>
            >(deps, cleanup);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/detail/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(0)
        #include BOOST_PP_ITERATE()

		template<
			typename TDependency
		  , typename TDeps
		  , typename TCleanup
		>
		static typename enable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
		acquire(TDeps& deps, TCleanup&) {
			return static_cast<TDependency&>(deps);
		}

		template<
			typename TDependency
		  , typename TDeps
		  , typename TCleanup
		>
		static typename disable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
		acquire(TDeps&, TCleanup& cleanup) {
		    static TDependency* dependency = 0;
			if (!dependency) {
				dependency = new TDependency();
				struct deleter {
					static void delete_ptr() {
						delete dependency;
						dependency = 0;
					}
				};
				cleanup.push_back(&deleter::delete_ptr);
			}
            return *dependency;
    	}
	};

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TCallStack
      , typename TDependency
      , typename TDeps
      , typename TCleanup
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == BOOST_PP_ITERATION()
      , typename TDependency::result_type
    >::type execute_impl(TDeps& deps, TCleanup& cleanup) {

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)       \
            BOOST_PP_COMMA_IF(n)                        \
            execute<                                    \
                typename mpl::at_c<                     \
                    typename ctor<TDependency>::type    \
                  , n                                   \
                >::type                                 \
              , TCallStack                              \
            >(deps, cleanup)

        return acquire<typename TDependency::type>(deps, cleanup).create(
            BOOST_PP_REPEAT(
                BOOST_PP_ITERATION()
              , BOOST_DI_CREATOR_EXECUTE
              , ~
            )
        );

        #undef BOOST_DI_CREATOR_EXECUTE
    }

#endif

