//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompuniversaling file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_DETAIL_WRAPPERS_UNIVERSAL_HPP

#include "boost/di/named.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/is_convertible_to_ref.hpp"

#include <vector>
#include <boost/type.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

template<typename T>
class universal_impl
{
public:
    template<typename TObject>
    explicit universal_impl(std::vector<boost::any>& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

private:
    std::vector<boost::any>& refs_;
    function<T()> callback_;
};

template<typename T>
class universal_impl<const T&>
{
public:
    template<typename TObject>
    explicit universal_impl(std::vector<boost::any>& refs, const TObject& object, typename enable_if<di::type_traits::is_convertible_to_ref<TObject, T&> >::type* = 0)
        : refs_(refs), callback_(boost::bind<const T&>(object, boost::type<T>())), m(false)
    { }

    template<typename TObject>
    explicit universal_impl(std::vector<boost::any>& refs, const TObject& object, typename disable_if<di::type_traits::is_convertible_to_ref<TObject, T&> >::type* = 0)
        : refs_(refs), callback_(boost::bind<const T&>(object, boost::type<T>())), m(true)
    { }

    operator const T&() const {
        if (m) {
            refs_.push_back(callback_());
            return any_cast<const T&>(refs_.back());
        }

        return callback_();
    }

private:
    std::vector<boost::any>& refs_;
    function<const T&()> callback_;
    bool m;
};

template<typename T, typename TName>
class universal_impl<named<T, TName> >
{
public:
    template<typename TObject>
    universal_impl(std::vector<boost::any>& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

    operator named<T, TName>() const {
        return callback_();
    }

private:
    std::vector<boost::any>& refs_;
    function<T()> callback_;
};

} // namespace detail

template<typename T>
class universal : public detail::universal_impl<T>
{
public:
    template<typename TObject>
    universal(std::vector<boost::any>& refs, const TObject& object)
        : detail::universal_impl<T>(refs, object)
    { }
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

