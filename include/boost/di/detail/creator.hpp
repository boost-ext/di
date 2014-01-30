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
    #include "boost/di/type_traits/is_same_base_of.hpp"

    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace detail {

template<typename T, typename = void>
struct named2
{
    typedef T type;
};

template<typename T>
struct named2<T, typename enable_if<
    has_named_type<typename type_traits::remove_accessors<T>::type> >::type
>
{
    typedef typename type_traits::remove_accessors<typename T::named_type>::type type;
};

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
            typename T
          , typename SS
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
        >
        struct eager_creator
        {
			typedef typename type_traits::make_plain<T>::type plain_t;

            eager_creator(TDeps& deps, TCleanup& cleanup)
                : deps_(deps), cleanup_(cleanup)
            { }

            template<
                typename U
			  , typename NU = typename named2<U>::type
			  , typename PU = typename type_traits::make_plain<U>::type
			  , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            >
            operator U() const {
                return creator::execute_impl<
                    NU
                  , SS
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<U, TCallStack>
                >(deps_, cleanup_)(boost::type<NU>());
            }

            TDeps& deps_;
            TCleanup& cleanup_;
        };

        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
        >
        struct eager_creator<T, mpl::true_, TCallStack, TDeps, TCleanup>
        {
            eager_creator(TDeps& deps, TCleanup& cleanup)
                : deps_(deps), cleanup_(cleanup)
            { }

            template< typename U >
            operator U() const {
                return creator::execute_impl<
                    U
                  , mpl::false_
                  , typename mpl::push_back<TCallStack, typename binder<T, TCallStack>::given>::type
                  , binder<U, TCallStack>
                >(deps_, cleanup_)(boost::type<U>());
            }

            TDeps& deps_;
            TCleanup& cleanup_;
        };

    public:
        template<
            typename T
          , typename SS
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
        >
        static eager_creator<T, SS, TCallStack, TDeps, TCleanup>
        execute(TDeps& deps, TCleanup& cleanup) {
            return eager_creator<T, SS, TCallStack, TDeps, TCleanup>(deps, cleanup);
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
		    static TDependency* dep = 0;
			if (!dep) {
				dep = new TDependency();
				struct deleter {
					static void delete_ptr() {
                        delete dep;
                        dep = 0;
					}
				};
                cleanup.push_back(&deleter::delete_ptr);
			}
            return *dep;
    	}

		template<typename C, typename T, typename = void>
		struct ccc_impl
		{
			typedef C type;
		};

		template<typename C, typename T>
		struct ccc_impl<C, T, typename enable_if<is_same<T, any_type> >::type>
		{
			typedef C type;
		};

		template<typename T, typename TDependency, int n>
		struct ccc
        	: ccc_impl<T, typename mpl::at_c<
                    typename ctor<TDependency>::type
                  , n
			  >::type
			>
		{ };

	};

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename SS
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
		typename ccc<T, TDependency, n>::type \
		, SS \
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

