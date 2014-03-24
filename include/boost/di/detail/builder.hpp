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
    #include "boost/di/convertibles/convertible.hpp"

    #include <typeinfo>
    #include <map>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    class builder
    {
        class type_comparator
        {
        public:
            bool operator()(const std::type_info* lhs, const std::type_info* rhs) const {
                return lhs->before(*rhs);
            }
        };

        typedef std::map<const std::type_info*, aux::shared_ptr<void>, type_comparator> scopes_type;

    public:
        explicit builder(scopes_type scopes = scopes_type())
            : scopes_(scopes)
        { }

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

            aux::shared_ptr<TDependency> dependency(new TDependency());
            scopes_[&typeid(TDependency)] = dependency;
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
      , typename TCreator
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    typename enable_if_c<
        mpl::size<TCtor>::value == BOOST_PP_ITERATION()
      , const convertibles::convertible<T>&
    >::type
    build(TCreator& creator, TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)   \
            BOOST_PP_COMMA_IF(n)                    \
            creator.template create<                \
               typename mpl::at_c<TCtor, n>::type   \
             , T                                    \
             , TCallStack                           \
             , TPolicies                            \
            >(deps, refs, visitor)

        aux::shared_ptr<convertibles::convertible<T> > convertible(
            new convertibles::convertible<T>(
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
            )
        );

        #undef BOOST_DI_CREATOR_EXECUTE

        refs.push_back(convertible);
        return *convertible;
    }

#endif

