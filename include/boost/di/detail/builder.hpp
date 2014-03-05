//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_BUILDER_HPP
    #define BOOST_DI_DETAIL_BUILDER_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/type_traits/make_plain.hpp"
    #include "boost/di/type_traits/is_same_base_of.hpp"

    #include "boost/di/concepts/type_traits/name.hpp"
    #include "boost/di/concepts.hpp"

    #include <typeinfo>
    #include <map>
    #include <vector>
    #include <typeinfo>
    #include <boost/config.hpp>
    #include <boost/bind.hpp>
    #include <boost/function.hpp>
    #include <boost/any.hpp>
    #include <boost/type.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/type_traits/is_polymorphic.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    template<typename T>
    struct convertible_
    {
        template<typename Q>
        convertible_(const Q& q)
            : f(boost::bind<T>(q, boost::type<T>()))
        { }

        operator T() const {
            return f();
        }

        function<T()> f;
    };

    template<typename T, typename TName>
    struct convertible_<named<T, TName> >
    {
        template<typename Q>
        convertible_(const Q& q)
            : f(boost::bind<T>(q, boost::type<T>()))
        { }

        operator T() const {
            return f();
        }

        operator named<T, TName>() const {
            return f();
        }

        function<T()> f;
    };

    template<typename T>
    struct convertible : convertible_<T>
    {
        template<typename Q>
        convertible(const Q& q)
            : convertible_<T>(q)
        { }
    };

    template<typename Creator>
    class builder
    {
        class type_comparator
        {
        public:
            bool operator ()(const std::type_info* lhs, const std::type_info* rhs) const {
                return lhs->before(*rhs);
            }
        };

        typedef std::map<const std::type_info*, aux::shared_ptr<void>, type_comparator> scopes_type;

    public:
        #define BOOST_PP_FILENAME_1 "boost/di/detail/builder.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

    private:
        template<
            typename TDependency
          , typename TDeps
        >
        typename enable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps& deps) {
            return static_cast<TDependency&>(deps);
        }

        template<
            typename TDependency
          , typename TDeps
        >
        typename disable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps&) {
            typename scopes_type::const_iterator it = scopes_.find(&typeid(TDependency));
            if (it != scopes_.end()) {
                return *static_cast<TDependency*>(it->second.get());
            }

            TDependency* dependency = new TDependency();
            scopes_[&typeid(TDependency)] = aux::shared_ptr<void>(dependency);
            return *dependency;
        }

        scopes_type scopes_;
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TCtor
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    typename enable_if_c<mpl::size<TCtor>::value == BOOST_PP_ITERATION(), const convertible<T>&>::type
    build(TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        typedef convertible<T> convertible_type;

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)           \
            BOOST_PP_COMMA_IF(n)                            \
            static_cast<Creator&>(*this).template create_<  \
               typename mpl::at_c<TCtor, n>::type           \
             , T                                            \
             , TCallStack                                   \
             , TPolicies                                    \
            >(deps, refs, visitor)

        convertible_type* convertible = new convertible_type(
            acquire<typename TDependency::type>(deps).create(
                type_traits::policy<
                    mpl::empty<typename TDeps::types>::value
                >()
                BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                BOOST_PP_REPEAT(
                    BOOST_PP_ITERATION()
                  , BOOST_DI_CREATOR_EXECUTE
                  , ~
                )
            )
        );

        #undef BOOST_DI_CREATOR_EXECUTE

        refs.push_back(aux::shared_ptr<void>(convertible));
        return *convertible;
    }

#endif

