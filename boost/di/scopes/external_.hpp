//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
    #define BOOST_DI_SCOPES_EXTERNAL_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/variant.hpp>
    #include <boost/type_traits/is_arithmetic.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/and.hpp>

    #include "boost/di/type_traits/value_type.hpp"
    #include "boost/di/type_traits/has_traits.hpp"
    #include "boost/di/named.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/scopes/external_.hpp"     \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace scopes {

    namespace aux {

    template<typename T, typename = void>
    struct plain_type
    {
        typedef T type;
    };

    template<typename T>
    struct plain_type<
        T
      , typename enable_if<
            mpl::and_<
                type_traits::has_name<T>
              , type_traits::has_value_type<T>
            >
        >::type
    >
    {
        typedef typename T::value_type type;
    };

    } // namespace aux

    template<
        typename T
      , typename TContext = mpl::vector0<>
      , typename = void
    >
    class external
    {
        typedef typename aux::plain_type<T>::type object_type;

        typedef variant<
            object_type&
          , const object_type&
          , shared_ptr<object_type>
        > value_type;

        template<typename>
        class external_impl;

        template<typename TDest>
        class external_impl<TDest&>
            : public static_visitor<TDest&>
        {
        public:
            template<typename TSrc>
            TDest& operator()(const TSrc& object) const {
                return const_cast<TDest&>(object);
            }

            template<typename TSrc>
            TDest& operator()(TSrc& object) const {
                return object;
            }

            template<typename TSrc>
            TDest& operator()(const shared_ptr<TSrc>& object) const {
                return *object;
            }
        };

        template<typename TDest>
        class external_impl<TDest*>
            : public static_visitor<TDest*>
        {
        public:
            template<typename TSrc>
            TDest* operator()(const TSrc& object) const {
                return new TSrc(object);
            }

            template<typename TSrc>
            TDest* operator()(TSrc& object) const {
                return new TSrc(object);
            }

            template<typename TSrc>
            TDest* operator()(const shared_ptr<TSrc>& object) const {
                return new TSrc(*object);
            }
        };

        template<typename TDest>
        class external_impl<shared_ptr<TDest> >
            : public static_visitor<shared_ptr<TDest> >
        {
        public:
            template<typename TSrc>
            shared_ptr<TDest> operator()(const TSrc& object) const {
                //return shared_ptr<TSrc>(new TSrc(object));
                return shared_ptr<TDest>();
            }

            template<typename TSrc>
            shared_ptr<TDest> operator()(TSrc& object) const {
                //return shared_ptr<TSrc>(new TSrc(&object));
                return shared_ptr<TDest>();
            }

            template<typename TSrc>
            shared_ptr<TDest> operator()(const shared_ptr<TSrc>& object) const {
                return object;
            }
        };

    public:
        typedef external type;
        typedef T element_type;
        typedef TContext context;

        class from
        {
        public:
            template<typename TValue>
            inline static external to(const TValue& value) {
                return external(value);
            }

            template<typename TValue>
            inline static external to(TValue& value) {
                return external(value);
            }

            template<typename TValue>
            inline static external to(shared_ptr<TValue> value) {
                return external(value);
            }
        };

        external(const object_type& object) // non explicit
            : external_(object)
        { }

        external(object_type& object) // non explicit
            : external_(object)
        { }

        template<typename TObject>
        external(shared_ptr<TObject> object) // non explicit
            : external_(object)
        { }

        operator object_type&() const {
            return apply_visitor(
                external_impl<object_type&>(), external_);
        }

        operator object_type*() const {
            return apply_visitor(
                external_impl<object_type*>(), external_);
        }

        template<typename I>
        operator shared_ptr<I>() const {
            return apply_visitor(
                external_impl<shared_ptr<object_type> >(), external_);
        }

        template<typename I, typename TName>
        operator named<shared_ptr<I>, TName>() const {
            return apply_visitor(
                external_impl<shared_ptr<object_type> >(), external_);
        }

    private:
        value_type external_;
    };

    template<
        typename T
      , typename TContext
    >
    class external<
        T
      , TContext
      , typename enable_if<
            mpl::or_<
                is_same<typename type_traits::value_type<T>::type, std::string>
              , is_arithmetic<typename type_traits::value_type<T>::type>
              , type_traits::has_result_type<typename type_traits::value_type<T>::type>
            >
        >::type
    >
    {
        typedef typename type_traits::value_type<T>::type value_type;
        typedef typename aux::plain_type<value_type>::type object_type;

    public:
        typedef external type;
        typedef T element_type;
        typedef TContext context;

        class from
        {
        public:
            template<typename TValue>
            inline static external to(const TValue& value) {
                return external(value);
            }

            template<typename TValue>
            inline static external to(TValue& value) {
                return external(value);
            }

            template<typename TValue>
            inline static external to(shared_ptr<TValue> value) {
                return external(value);
            }
        };

        external(object_type object) // non explicit
            : external_(object)
        { }

        template<typename TObject>
        external(shared_ptr<TObject> object) // non explicit
            : external_(*object)
        { }

        operator object_type() const {
            return external_;
        }

        operator object_type*() const {
            return new object_type(external_);
        }

        operator shared_ptr<object_type>() const {
            return make_shared<object_type>(external_);
        }

        template<typename TName>
        operator named<shared_ptr<object_type>, TName>() const {
            return make_shared<object_type>(external_);
        }

        template<typename TName>
        operator named<object_type, TName>() const {
            return external_;
        }

    private:
        object_type external_;
    };

    class external_
    {
    public:
        template<
            typename TExpected
          , typename TGiven = TExpected
        >
        class scope
        {
        public:
            typedef external<TExpected> result_type;

            result_type create() {
                return boost::shared_ptr<TGiven>();
            }

            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        return boost::shared_ptr<TGiven>();
    }

#endif

