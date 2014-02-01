//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/make_plain.hpp"
    #include "boost/di/type_traits/is_same_base_of.hpp"

    #include <boost/type.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {
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
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct dependency
        {
            typedef T type;
            typedef TCallStack context;
            typedef typename TDependency::given given;
            typedef typename TDependency::expected expected;
            typedef typename TDependency::scope scope;
        };

        BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
          , typename TVisitor
        >
        class eager_creator
        {
			typedef typename type_traits::make_plain<T>::type plain_t;

			template<typename U, typename = void>
			struct named_type
			{
				typedef U type;
			};

			template<typename U>
			struct named_type<U, typename enable_if<
				has_named_type<typename type_traits::remove_accessors<U>::type> >::type
			>
			{
				typedef typename type_traits::remove_accessors<typename U::named_type>::type type;
			};

		public:
            eager_creator(TDeps& deps, TCleanup& cleanup, const TVisitor& visitor)
                : deps_(deps), cleanup_(cleanup), visitor_(visitor)
            { }

            template<
                typename U
              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            >
            operator U() {
                return creator::execute_impl<
                    NU
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<U, TCallStack>
                >(deps_, cleanup_, visitor_)(boost::type<NU>());
            }

            template<
                typename U
              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            >
            operator const U&() const {
                return creator::execute_impl<
                    const U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<const U&, TCallStack>
                >(deps_, cleanup_, visitor_)(boost::type<const NU&>());
            }

            template<
                typename U
              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            >
            operator U&() const {
                return creator::execute_impl<
                    U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<U&, TCallStack>
                >(deps_, cleanup_, visitor_)(boost::type<NU&>());
            }

		private:
            TDeps& deps_;
            TCleanup& cleanup_;
            const TVisitor& visitor_;
        };

    public:
        template<
            typename T
          , typename TParent
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
          , typename TVisitor
        >
        static eager_creator<TParent, TCallStack, TDeps, TCleanup, TVisitor>
        execute(TDeps& deps, TCleanup& cleanup, const TVisitor& visitor, typename enable_if<is_same<T, any_type>>::type* = 0) {
            return eager_creator<TParent, TCallStack, TDeps, TCleanup, TVisitor>(deps, cleanup, visitor);
        }

        template<
            typename T
          , typename
          , typename TCallStack
          , typename TDeps
          , typename TCleanup
          , typename TVisitor
        >
        static typename binder<T, TCallStack>::result_type
		execute(TDeps& deps, TCleanup& cleanup, const TVisitor& visitor, typename disable_if<is_same<T, any_type>>::type* = 0) {
            return execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , binder<T, TCallStack>
            >(deps, cleanup, visitor);
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
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == BOOST_PP_ITERATION()
      , typename TDependency::result_type
    >::type execute_impl(TDeps& deps, TCleanup& cleanup, const TVisitor& visitor) {

		(visitor)(dependency<T, TCallStack, TDependency>());

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)       \
            BOOST_PP_COMMA_IF(n)                        \
            execute<                                    \
			   typename mpl::at_c< 						\
				   typename ctor<TDependency>::type 	\
				 , n 									\
			   >::type 									\
			  , T 										\
              , TCallStack                              \
            >(deps, cleanup, visitor)

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

