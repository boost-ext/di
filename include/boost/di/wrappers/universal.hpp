//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_DETAIL_WRAPPERS_UNIVERSAL_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

#include <vector>
#include <boost/type.hpp>
#include <boost/non_type.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

template<typename T>
class universal_impl
{
public:
    template<typename TObject>
    explicit universal_impl(std::vector<aux::shared_ptr<void> >&, const TObject& object)
        : callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

private:
    function<T()> callback_;
};

template<typename T>
class universal_impl<const T&>
{
    template<typename TValueType>
    struct holder
    {
        explicit holder(const TValueType& value)
            : held(value)
        { }

        TValueType held;
    };

    template<typename TValueType, typename TSignature>
    class is_convertible
    {
        template<typename U> static mpl::aux::yes_tag test(non_type<TSignature, &U::operator()>*);
        template<typename>   static mpl::aux::no_tag test(...);

    public:
        typedef is_convertible type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<TValueType>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename TObject, typename TValueType>
    struct is_convertible_to_ref
        : mpl::or_<
              is_convertible<TObject, TValueType&(TObject::*)(const type<TValueType&>&) const>
            , is_convertible<TObject, const TValueType&(TObject::*)(const type<const TValueType&>&) const>
          >
    { };

public:
    template<typename TObject>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TObject& object
                 , typename enable_if<is_convertible_to_ref<TObject, T> >::type* = 0)
        : refs_(refs), callback_ref_(boost::bind<const T&>(object, boost::type<const T&>()))
    { }

    template<typename TObject>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TObject& object
                 , typename disable_if<is_convertible_to_ref<TObject, T> >::type* = 0)
        : refs_(refs), callback_copy_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator const T&() const {
        if (!callback_copy_.empty()) {
            aux::shared_ptr<holder<T> > value(new holder<T>(callback_copy_()));
            refs_.push_back(value);
            return value->held;
        }

        return callback_ref_();
    }

private:
    std::vector<aux::shared_ptr<void> >& refs_;
    function<T()> callback_copy_;
    function<const T&()> callback_ref_;
};

template<typename T, typename TName>
class universal_impl<named<T, TName> >
{
public:
    template<typename TObject>
    universal_impl(std::vector<aux::shared_ptr<void> >&, const TObject& object)
        : callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

    operator named<T, TName>() const {
        return callback_();
    }

private:
    function<T()> callback_;
};

} // namespace detail

template<typename T>
class universal : public detail::universal_impl<T>
{
public:
    template<typename TObject>
    universal(std::vector<aux::shared_ptr<void> >& refs, const TObject& object)
        : detail::universal_impl<T>(refs, object)
    { }
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

