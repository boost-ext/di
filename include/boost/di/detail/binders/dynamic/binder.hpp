//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_DYNAMIC_BUILDER_HPP
#define BOOST_DI_DETAIL_DYNAMIC_BUILDER_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/create_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/is_same_base_of.hpp"

#include "boost/di/concepts/type_traits/name.hpp"
#include "boost/di/concepts.hpp"
#include "boost/di/detail/builder.hpp"

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

template<typename, typename Creator>
class dynamic_binder
    : public builder<Creator>
{
    typedef std::vector< function<any()> > fun_type;
    typedef std::vector<std::pair<function<int(const std::type_info*, const std::type_info*, const std::vector<const std::type_info*>&)>, fun_type> > creators_type;

    template<typename TC, typename T>
    struct gett
    {
        typedef T type;
    };

    template<typename TC, typename T>
    struct gett<convertibles::shared<TC>, T>
    {
        typedef aux::shared_ptr<T> type;
    };

public:
    dynamic_binder()
        : skip_(&typeid(void))
    { }

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
    const convertible<T>&
    resolve_(TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        typedef convertible<T> convertible_type;

        int best = 0;
        std::size_t r = 0;

        if (skip_ != &typeid(T)) {
            skip_ = &typeid(void);

            for (std::size_t q = 0; q < creators_.size(); ++q) {

                std::vector<const std::type_info*> call_stack;
                fill_call_stack<TCallStack>(call_stack);

                int current = creators_[q].first(
                    &typeid(typename type_traits::make_plain<T>::type)
                  , &typeid(typename concepts::type_traits::get_name<T>::type)
                  , call_stack
                );

                if (current > best) {
                    best = current;
                    r = q;
                }
            }

            std::cout << typeid(T).name() <<  ": " <<  best << std::endl;
            if (best > 0) {
                for (fun_type::const_iterator it = creators_[r].second.begin(); it != creators_[r].second.end(); ++it) {
                    if ((*it)().type() == typeid(convertible<T>)) {

                        convertible_type* convertible(new convertible_type(
                            any_cast<convertible_type>((*it)())
                        ));

                        refs.push_back(aux::shared_ptr<void>(convertible));
                        return *convertible;
                    }
                }

                throw std::runtime_error("conversion not allowed");
            }
        }

        detail::pool<> p;
        return this->template build<
            T
          , TCtor
          , TCallStack
          , TPolicies
          , TDependency
        >(p, refs, visitor);
    }

    class empty_visitor
    {
    public:
        template<typename T>
        void operator()(const T&) const { }
    };

    template<typename T, typename TInjector>
    void bind_dependency(TInjector injector) {
        typedef mpl::vector0<> policies;
        typedef mpl::vector0<> call_stack;
        std::vector<aux::shared_ptr<void> > refs_;

        bind_create<T, policies, call_stack>(
            *this
          , static_cast<pool<typename TInjector::deps>&>(injector)
          , refs_
          , empty_visitor()
        );
    }

private:
    template<
        typename TDependency
      , typename TPolicies
      , typename TCallStack
      , typename TCreator
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    void bind_create(const TCreator& creator, TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        typedef typename TDependency::expected type;

        typedef typename mpl::if_<
            is_same<typename TDependency::name, di::concepts::detail::no_name>
          , typename mpl::if_<
                is_polymorphic<type>
              , aux::shared_ptr<type>
              , typename gett<typename TDependency::result_type, type>::type
            >::type
          , typename mpl::if_<
                is_polymorphic<type>
              , named<aux::shared_ptr<type>, typename TDependency::name>
              , named<typename gett<typename TDependency::result_type, type>::type, typename TDependency::name>
            >::type
        >::type t;

        fun_type v;

        v.push_back(
              boost::bind(
                   &TCreator::template create_any<
                       t
                     , t
                     , typename TDependency::context
                     , TPolicies
                     , TDeps
                     , TRefs
                     , TVisitor
                   >
                 , creator
                 , deps
                 , refs
                 , visitor
               )
              );

        creators_.push_back(
            std::make_pair(
                boost::bind(&TDependency::when, _1, _2, _3)
              , v
            )
        );
    }

    template<
        typename T
      , typename TParent // to ignore copy/move ctor
      , typename TCallStack
      , typename TPolicies
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    any create_any(TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        skip_ = &typeid(T);
        return any(static_cast<Creator&>(*this).template create_<T, TParent, TCallStack, TPolicies>(deps, refs, visitor));
    }

    template<typename TSeq, typename V>
    static typename enable_if<mpl::empty<TSeq> >::type fill_call_stack(V&) { }

    template<typename TSeq, typename V>
    static typename disable_if<mpl::empty<TSeq> >::type fill_call_stack(V& v) {
        v.push_back(&typeid(typename mpl::front<TSeq>::type));
        fill_call_stack<typename mpl::pop_front<TSeq>::type>(v);
    }

    creators_type creators_;
    const std::type_info* skip_;
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

